#include <iostream>
#include <ucontext.h>
#include <boost/context/detail/fcontext.hpp>

using namespace std;


boost::context::detail::fcontext_t m_ctx;
boost::context::detail::transfer_t m_main;
// clang++ testBoostContext.cpp -g -o testBoostContext -lboost_context 
// -I/usr/local/boost/include -L/usr/local/boost/lib
static void func_boost(boost::context::detail::transfer_t p)
{
    cout << "func()" << endl;
    cout << m_main.fctx << endl;
    cout << m_main.data << endl;
    cout << p.fctx << endl;
    boost::context::detail::jump_fcontext(p.fctx, nullptr);
}

static ucontext_t ctx[3];

void func1(void)
{
    cout << "func1 on doing" << endl;
    swapcontext(&ctx[1], &ctx[2]);
    cout << "func1 finished" << endl;
}

void func2(void)
{
    cout << "func2 on doing" << endl;
    swapcontext(&ctx[2], &ctx[1]);
    cout << "func2 finished" << endl;
}

int main()
{
    
    char* m_stack = new char[8*102400];
    int m_stacksize = 102400;
    m_ctx = boost::context::detail::make_fcontext(
        (char*)m_stack+m_stacksize, m_stacksize, &func_boost);
    
    // char* m_mainstack = new char[102400];
    // int m_mainstacksize = 102400;
    // m_main.fctx = boost::context::detail::make_fcontext(
    //     (char*)m_mainstack, m_mainstacksize, nullptr);

    m_main = boost::context::detail::jump_fcontext(m_ctx, nullptr);

    cout << m_main.fctx << endl;
    cout << "main end" << endl;





    char stack1[8*1024];
    // getcontext(&ctx[1]);
    ctx[1].uc_stack.ss_sp = stack1;
    ctx[1].uc_stack.ss_size = sizeof(stack1);
    ctx[1].uc_link = &ctx[0];
    makecontext(&ctx[1], func1, 0);

    cout << "main on doing" << endl;
    swapcontext(&ctx[0], &ctx[2]);
    cout << "main finished" << endl;
    return 0;
}