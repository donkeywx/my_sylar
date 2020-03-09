#include "../sylar.h"
#include <vector>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int count = 0;
sylar::RWLock rwlokc;
sylar::Mutex mutex;
void func1()
{
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                            << " this.name: " << sylar::Thread::GetThis()->getName()
                            << " id: " << sylar::getThreadId()
                            << " this.id: " << sylar::Thread::GetThis()->getId();
    
    // // sleep(1);
    // for(int i = 0; i < 100000; ++i) {
    //     sylar::RWLock::WriteLock wrlock(rwlokc);
    //     // sylar::Mutex::Lock lock(mutex);
    //     ++count;
    // }
}

int main()
{
    std::vector<sylar::Thread::ptr> threads;

    for (int i = 0; i < 10; i++)
    {
        sylar::Thread::ptr thread(new sylar::Thread(&func1));
        threads.push_back(thread);
    }

    for(size_t i = 0; i < threads.size(); ++i) 
    {
        threads[i]->join();
    }

    std::cout << count << std::endl;
    // SYLAR_LOG_INFO(g_logger) << "thread test end";
    // SYLAR_LOG_INFO(g_logger) << "count=" << count;
}