#include "fiber.h"
#include "log.h"
#include "config.h"
#include "macro.h"
#include <atomic>

namespace sylar
{
static Logger::ptr g_logger = SYLAR_LOG_NAME("system");

static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count {0};

static thread_local Fiber* t_fiber = nullptr;   // 当前线程的当前协程
static thread_local Fiber::ptr t_threadFiber = nullptr; // 主协程

static ConfigVar<uint32_t>::ptr g_fiber_stack_size = Config::Lookup<uint32_t>("fiber.stack_size", 128*1024, "fiber stack size");

class MallocStackAllocator
{
public:
    static void* Alloc(size_t size)
    {
        return malloc(size);
    }

    static void Dealloc(void* vp, size_t size)
    {
        return free(vp);
    }
};

Fiber::Fiber()
{
    m_state = EXEC;
    SetThis(this);  // 当前的协程
    
    if (getcontext(&m_ctx))
    {
        SYLAR_ASSERT2(false, "gecontext");
    }

    s_fiber_count++;

    SYLAR_LOG_DEBUG(g_logger) << "Fiber: Fiber main";
}

Fiber::Fiber(std::function<void()> cb, size_t stackSize, bool useCaller)
    :m_id(++s_fiber_id), m_cb(cb)
{
    s_fiber_count++;

    m_stackSize = stackSize ? stackSize : g_fiber_stack_size->getVal();

    m_stack = MallocStackAllocator::Alloc(m_stackSize);

    if (getcontext(&m_ctx))
    {
        SYLAR_ASSERT2(false, "gecontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stackSize;

    if (!useCaller)
    {
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
    }
    else
    {
        makecontext(&m_ctx, &Fiber::CallerMainFunc, 0);
    }

    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber id = " << m_id;
    
}

Fiber::~Fiber()
{
    s_fiber_count--;
    if (m_stack)
    {
        SYLAR_ASSERT(INIT == m_state
            || TERM == m_state
            || EXEC == m_state);

        MallocStackAllocator::Dealloc(m_stack, m_stackSize);
    }
    else // 没有栈，可认为是主协程
    {
        SYLAR_ASSERT(!m_cb);    // 主协程没有cb，并且状态为exec
        SYLAR_ASSERT(EXEC == m_state);

        Fiber* cur = t_fiber;   // 要查看线程的主fiber是否等于自己
        if (this == cur)
        {
            SetThis(nullptr);
        }
    }

    SYLAR_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << m_id
            << " total=" << s_fiber_count;
    
}

void Fiber::reset(std::function<void()> cb)
{
    SYLAR_ASSERT(m_stack);
    SYLAR_ASSERT(INIT == m_state
            || TERM == m_state
            || EXEC == m_state);
    
    m_cb = cb;
    if (getcontext(&m_ctx))
    {
        SYLAR_ASSERT2(false, "gecontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stackSize;
    makecontext(&m_ctx, &Fiber::MainFunc, 0);

    m_state = INIT;
}

// 切换到当前协程执行
void Fiber::swapIn()
{
    SetThis(this);
    SYLAR_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if(swapcontext(&(t_threadFiber->m_ctx), &m_ctx))
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

// 切换到后台
void Fiber::swapOut()
{
    SetThis(t_threadFiber.get());
    if(swapcontext(&m_ctx, &(t_threadFiber->m_ctx)))
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

void Fiber::call()
{
    SetThis(this);
    m_state = EXEC;

    if (swapcontext(&t_threadFiber->m_ctx, &m_ctx)) // 执行当前协程，保存当前线程的主协程
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

void Fiber::back()
{
    SetThis(t_threadFiber.get());
    if(swapcontext(&m_ctx, &t_threadFiber->m_ctx)) // 终止当前协程，执行当前线程的主协程
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

// 设置当前协程
void Fiber::SetThis(Fiber* f)
{
    t_fiber = f;
}

Fiber::ptr Fiber::GetThis()
{
    if (t_fiber)
    {
        return t_fiber->shared_from_this();
    }

    Fiber::ptr mainFiber(new Fiber);
    SYLAR_ASSERT(t_fiber == mainFiber.get());
    t_threadFiber = mainFiber;

    return t_fiber->shared_from_this();
}

void Fiber::YieldToReady()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(EXEC == cur->m_state);
    cur->m_state = READY;
    cur->swapOut();
}

// 当前协程切换到后台，并置为ready状态
void Fiber::YieldToHold()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(EXEC == cur->m_state);
    cur->m_state = HOLD;
    cur->swapOut();
}

uint64_t Fiber::TotalFibers()
{
    return s_fiber_count;
}

void Fiber::MainFunc()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur);
    try
    {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    }
    catch(const std::exception& e)
    {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except: " << e.what()
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    }
    catch(...)
    {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    }
    // 这里切换到主要协程之后，子协程中的Fiber::ptr 的引用没有被减1，所以就会内存泄漏
    // 这就是为什么要用reset的原因
    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->swapOut(); // 等待协程执行完毕之后，这里会再次切到主协程执行

    SYLAR_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->getId()));
    
}

void Fiber::CallerMainFunc()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur);
    try {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    } 
    catch (std::exception& ex) 
    {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    }
    catch (...) 
    {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->back();
    SYLAR_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->getId()));
}

uint64_t Fiber::GetFiberId()
{
    if(t_fiber) 
    {
        return t_fiber->getId();
    }
    return 0;
}
}