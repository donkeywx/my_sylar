#include "fiber.h"
#include "config.h"
#include "macro.h"
#include "log.h"
#include "scheduler.h"
#include "util.h"
#include <atomic>

namespace sylar {

static Logger::ptr g_logger = SYLAR_LOG_NAME("system");

static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count {0};

// 线程当前运行的协程
static thread_local Fiber* t_threadCurFiber = nullptr;
// 线程的调度协程
static thread_local Fiber::ptr t_threadScheFiber = nullptr;
// 线程的原context，为调度线程使用
static thread_local ucontext_t t_oldThreadContext;

static ConfigVar<uint32_t>::ptr g_fiber_stack_size =
    Config::Lookup<uint32_t>("fiber.stack_size", 128 * 1024, "fiber stack size");

class MallocStackAllocator {
public:
    static void* Alloc(size_t size) {
        return malloc(size);
    }

    static void Dealloc(void* vp, size_t size) {
        return free(vp);
    }
};

using StackAllocator = MallocStackAllocator;

uint64_t Fiber::GetFiberId()
{
    if(t_threadCurFiber)
    {
        return t_threadCurFiber->getId();
    }
    return 0;
}

Fiber::Fiber()
{
    m_state = EXEC;
    SetThis(this);

    if(getcontext(&m_ctx))
    {
        SYLAR_ASSERT2(false, "getcontext");
    }

    ++s_fiber_count;

    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber main";
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller)
    : m_id(++s_fiber_id), m_cb(cb)
{
    ++s_fiber_count;
    m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getValue();

    m_stack = StackAllocator::Alloc(m_stacksize);
    if(getcontext(&m_ctx)) {
        SYLAR_ASSERT2(false, "getcontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    if(!use_caller) {
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
    } else {
        makecontext(&m_ctx, &Fiber::CallerMainFunc, 0);
    }

    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber id=" << m_id;
}

Fiber::~Fiber()
{
    --s_fiber_count;
    if(m_stack)
    {
        SYLAR_ASSERT(m_state == TERM
                || m_state == EXCEPT
                || m_state == INIT);

        StackAllocator::Dealloc(m_stack, m_stacksize);
    }
    else
    {
        SYLAR_ASSERT(!m_cb);
        SYLAR_ASSERT(m_state == EXEC);

        Fiber* cur = t_threadCurFiber;
        if(cur == this)
        {
            SetThis(nullptr);
        }
    }
    SYLAR_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << m_id
                              << " total=" << s_fiber_count;
}

//重置协程函数，并重置状态
//INIT，TERM, EXCEPT
void Fiber::reset(std::function<void()> cb)
{
    SYLAR_ASSERT(m_stack);
    SYLAR_ASSERT(m_state == TERM
            || m_state == EXCEPT
            || m_state == INIT);
    m_cb = cb;
    if(getcontext(&m_ctx)) {
        SYLAR_ASSERT2(false, "getcontext");
    }

    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    makecontext(&m_ctx, &Fiber::MainFunc, 0);
    m_state = INIT;
}

void Fiber::call()
{
    SetThis(this);
    m_state = EXEC;
    if(swapcontext(&t_oldThreadContext, &m_ctx))
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

void Fiber::back()
{
    SetThis(t_threadScheFiber.get());
    if(swapcontext(&m_ctx, &t_oldThreadContext))
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

//切换到当前协程执行
void Fiber::swapIn()
{
    SetThis(this);
    SYLAR_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if(swapcontext(&t_threadScheFiber->m_ctx, &m_ctx))
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

//切换到后台执行
void Fiber::swapOut()
{
    SetThis(t_threadScheFiber.get());
    if(swapcontext(&m_ctx, &t_threadScheFiber->m_ctx))
    {
        SYLAR_ASSERT2(false, "swapcontext");
    }
}

void Fiber::CreateScheFiber(Fiber::ptr scheFiber)
{
    if (nullptr != t_threadScheFiber)
    {
        return;
    }
    if (nullptr != scheFiber)
    {
        SetThis(scheFiber.get());
        t_threadScheFiber = scheFiber;
        return;
    }

    Fiber::ptr fiber(new Fiber);
    SYLAR_ASSERT(t_threadCurFiber == fiber.get());
    t_threadScheFiber = fiber;
}

//设置当前协程
void Fiber::SetThis(Fiber* f)
{
    t_threadCurFiber = f;
}

//返回或创建返回当前协程
Fiber::ptr Fiber::GetThis()
{
    if(t_threadCurFiber)
    {
        return t_threadCurFiber->shared_from_this();
    }

    // 当前线程还无协程，则创建线程调度协程，返回线程当前协议，也就是调度协程
    // Fiber::ptr scheFiber(new Fiber);
    // SYLAR_ASSERT(t_threadCurFiber == scheFiber.get());
    // t_threadScheFiber = scheFiber;
    return nullptr;
}

//协程切换到后台，并且设置为Ready状态
void Fiber::YieldToReady()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur->m_state == EXEC);
    cur->m_state = READY;
    cur->swapOut();
}

//协程切换到后台，并且设置为Hold状态
void Fiber::YieldToHold()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur->m_state == EXEC);
    cur->m_state = HOLD;
    cur->swapOut();
}

//总协程数
uint64_t Fiber::TotalFibers()
{
    return s_fiber_count;
}

void Fiber::MainFunc()
{
    Fiber::ptr cur = GetThis();
    SYLAR_ASSERT(cur);
    try {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    } catch (std::exception& ex) {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    } catch (...) {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->swapOut();

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
    } catch (std::exception& ex) {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    } catch (...) {
        cur->m_state = EXCEPT;
        SYLAR_LOG_ERROR(g_logger) << "Fiber Except"
            << " fiber_id=" << cur->getId()
            << std::endl
            << sylar::Backtrace2String();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    SYLAR_LOG_ERROR(g_logger) << "back";
    raw_ptr->back();
    SYLAR_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->getId()));

}

}
