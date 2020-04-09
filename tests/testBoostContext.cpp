#include <iostream>
#include <stdio.h>
#include "../sylar.h"

void func();
void func_2();
sylar::Context context(func, 10240);




int main(int argc, char** argv)
{
    
    context.swapIn();
    // context.swapIn();
    std::cout << "end" << std::endl;
    
}
void func_2()
{
    printf("aa2\n");
}
void func()
{
    printf("aa\n");
    std::cout << "hello" << std::endl;
    // context.swapOut();
    func_2();
}


// #include <iostream>
// #include <stdio.h>
// #include <ucontext.h>
// #include <boost/context/detail/fcontext.hpp>

// using namespace std;

// // clang++ testBoostContext.cpp -g -o testBoostContext -lboost_context -I/usr/local/boost/include -L/usr/local/boost/lib

// static void func_2()
// {
//     printf("hello \n");
// }

// static void func(boost::context::detail::transfer_t p)
// {
//     printf("hello func()\n");
//     std::cout << p.fctx << std::endl;
//     boost::context::detail::jump_fcontext(p.fctx, nullptr);
//     func_2();
// }

// // boost context
// boost::context::detail::fcontext_t m_ctx;
// boost::context::detail::transfer_t m_main;
// static void func_boost(boost::context::detail::transfer_t p)
// {
//     // cout << "func_boost()" << endl;
//     boost::context::detail::jump_fcontext(p.fctx, nullptr);
// }

// // ucontext
// static ucontext_t ctx[2];
// static void func_ucontext()
// {
//     // cout << "func_ucontext()" << endl;
//     swapcontext(&ctx[1], &ctx[0]);
// }


// int main()
// {

//     char* m_stack = new char[10240];
//     int m_stacksize = 10240;

//     boost::context::detail::fcontext_t m_ctx = boost::context::detail::make_fcontext(
//                 (char*)m_stack+m_stacksize, m_stacksize, &func);
//     boost::context::detail::transfer_t p = 
//         boost::context::detail::jump_fcontext(m_ctx, nullptr);
//     std::cout << m_ctx << std::endl;
//     std::cout << p.fctx << std::endl;
//     std::cout << "end" << std::endl;
    
//     // char* m_stack = new char[8*102400];
//     // int m_stacksize = 8*102400;

//     // time_t start, end;w
//     // start = clock();

//     // // 100万 0.022593S
//     // for (int i = 0; i < 100; i++)
//     // {
//     //     for (int j = 0; j < 10000; j++)
//     //     {
//     //         m_ctx = boost::context::detail::make_fcontext(
//     //             (char*)m_stack+m_stacksize, m_stacksize, &func_boost);

//     //         m_main = boost::context::detail::jump_fcontext(m_ctx, nullptr);
//     //     }
//     // }


//     // 100万 0.603699S
//     // for (int i = 0; i < 100; i++)
//     // {
//     //     for (int j = 0; j < 10000; j++)
//     //     {
//     //         getcontext(&ctx[1]);
//     //         ctx[1].uc_stack.ss_sp = m_stack;
//     //         ctx[1].uc_stack.ss_size = m_stacksize;
//     //         ctx[1].uc_link = nullptr;
//     //         makecontext(&ctx[1], func_ucontext, 0);
//     //         swapcontext(&ctx[0], &ctx[1]);   // 保存到ctx0，切换到ctx1
//     //     }
//     // }

//     // end = clock();
//     // std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
//     // cout << "main end" << endl;
//     // return 0;
// }