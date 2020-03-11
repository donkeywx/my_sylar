#include "../sylar.h"

sylar::Logger::ptr logger = SYLAR_LOG_NAME("system");


void run_in_fiber()
{
    SYLAR_LOG_INFO(logger) << "run_in_fiber start";
    // sylar::Fiber::YieldToHold();
    SYLAR_LOG_INFO(logger) << "run_in_fiber end";
    // sylar::Fiber::YieldToHold();
}

int main()
{
    sylar::Fiber::GetThis();    // 确保当前线程有主协程
    SYLAR_LOG_ERROR(logger) << "main begin";
    sylar::Fiber::ptr fiber(new sylar::Fiber(run_in_fiber));
    fiber->swapIn();
    // SYLAR_LOG_ERROR(logger) << "main after swap in";
    // fiber->swapIn();
    SYLAR_LOG_ERROR(logger) << "main end";
    return 0;

    // sylar::Thread::SetName("main");

    // std::vector<sylar::Thread::ptr> thrs;
    // for(int i = 0; i < 3; ++i) {
    //     thrs.push_back(sylar::Thread::ptr(
    //                 new sylar::Thread(&test_fiber, "name_" + std::to_string(i))));
    // }
    // for(auto i : thrs) {
    //     i->join();
    // }
}

// #include <iostream>
// #include <ucontext.h>

// void func()
// {
//     std::cout << "hello ucontext" << std::endl;
// }

// void ucontextTest()
// {
//     char stack[1024* 1024];

//     ucontext_t utx;
//     ucontext_t mainUtx;

//     getcontext(&utx);

//     utx.uc_link = nullptr;
//     utx.uc_stack.ss_sp = stack;
//     utx.uc_stack.ss_size = sizeof(stack);

//     makecontext(&utx, func, 0);
    
//     swapcontext(&mainUtx, &utx);

//     std::cout << "ucontextTest()" << std::endl;
// }

// int main()
// {
//     ucontextTest();

//     std::cout << "end" << std::endl;

//     return 0;
// }