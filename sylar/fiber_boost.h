#ifndef __SYLAR_FIBER_BOOST_H__
#define __SYLAR_FIBER_BOOST_H__

#include "context.h"

namespace sylar
{
class BoostFiber: public Noncopyable, public std::enable_shared_from_this<BoostFiber>
{
public:
    typedef std::shared_ptr<BoostFiber> ptr;
    /**
     * @brief 协程状态
     */
    enum State
    {
        /// 初始化状态
        INIT,
        /// 暂停状态
        HOLD,
        /// 执行中状态
        EXEC,
        /// 结束状态
        TERM,
        /// 可执行状态
        READY,
        /// 异常状态
        EXCEPT
    };

    /**
     * @brief 构造函数
     * @param[in] cb 协程执行的函数
     * @param[in] stacksize 协程栈大小
     * @param[in] use_caller 是否在MainFiber上调度
     */
    BoostFiber(Context::Callback _cb, size_t stackSize = 1024);

    /**
     * @brief 析构函数
     */
    ~BoostFiber();

    void swapIn();

    void swapOut();

private:
    Context m_context;
};
}

#endif