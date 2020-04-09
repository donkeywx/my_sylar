#include "context.h"
#include <iostream>
#include <stdio.h>
namespace sylar
{

static thread_local boost::context::detail::fcontext_t tls_context;

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

Context::Context(Callback cb, uint32_t stackSize)
    : m_cb(cb), m_stackSize(stackSize)
{
    m_stack = (char*) MallocStackAllocator::Alloc(stackSize);
    
    m_ctx = boost::context::detail::make_fcontext(
            m_stack + m_stackSize, m_stackSize, Context::Run);
}

Context::~Context()
{
    std::cout << "~Context" << std::endl;
    MallocStackAllocator::Dealloc(m_stack, m_stackSize);
}

void Context::swapIn()
{
    m_ctx = boost::context::detail::jump_fcontext(m_ctx, this).fctx;
}

void Context::swapOut()
{
    tls_context = boost::context::detail::jump_fcontext(tls_context, nullptr).fctx;
}

boost::context::detail::fcontext_t& Context::GetTlsContext()
{
    return tls_context;
}

void Context::Run(boost::context::detail::transfer_t arg)
{
    tls_context = arg.fctx;
    Context* cur = static_cast<Context*> (arg.data);
    cur->m_cb();
    cur->m_cb = nullptr;

    cur->swapOut();
}
}