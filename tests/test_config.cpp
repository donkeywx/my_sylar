#include "../sylar/config.h"
#include "../sylar/log.h"

static sylar::ConfigVar<int>::ptr global = 
    sylar::Config::Lookup("port", (int)8080, "port");

int main()
{
    SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << global->getVal();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << global->toString();
    return 0;
}