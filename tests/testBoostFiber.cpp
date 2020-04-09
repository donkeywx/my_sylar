#include "../sylar.h"
#include <iostream>

void cb()
{
    std::cout << "hello boost fiber" << std::endl;    
}


class A
{
public:
    ~A(){std::cout << "A" << std::endl;}
};
class B
{
public:
    ~B(){std::cout << "B" << std::endl;}
private:
    A a;
};
int main(int argc, char** argv)
{
    sylar::BoostFiber fiber(cb);
    fiber.swapIn();
    std::cout << "main end" << std::endl;
    B b;
    return 0;
}