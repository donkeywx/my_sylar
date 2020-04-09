#ifndef __SYLAR_CONTEXT_H__
#define __SYLAR_CONTEXT_H__

#include <memory>
#include <functional>
#include "noncopyable.h"
#include <boost/context/detail/fcontext.hpp>

namespace sylar
{
class Context: public Noncopyable, public std::enable_shared_from_this<Context>
{
public:
    typedef std::shared_ptr<Context> Ptr;
    typedef std::function<void()> Callback;

    Context(std::function<void()> cb, uint32_t stackSize);
    ~Context();

    void swapIn();
    void swapOut();

private:
    // 
    boost::context::detail::fcontext_t& GetTlsContext();
    static void Run(boost::context::detail::transfer_t arg);
private:
    // 上下文
    boost::context::detail::fcontext_t m_ctx;
    // 回调函数
    Callback m_cb;
    // 协程运行堆栈
    char* m_stack = nullptr;
    // 协程运行堆栈大小
    uint32_t m_stackSize = 0;
};
}

#endif