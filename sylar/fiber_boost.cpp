#include "fiber_boost.h"
#include <atomic>
#include <iostream>

namespace sylar
{
BoostFiber::BoostFiber(Context::Callback cb, size_t stackSize)
    : m_context(cb, stackSize)
{
    
}

BoostFiber::~BoostFiber()
{
    std::cout << "~BoostFiber" << std::endl;
}

void BoostFiber::swapIn()
{
    m_context.swapIn();
}

void BoostFiber::swapOut()
{
    m_context.swapOut();
}

}

