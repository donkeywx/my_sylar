#include "thread.h"
#include "log.h"
#include "util.h"

namespace sylar
{

// 运行时参数
static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOWN";

// static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

Thread::Thread(std::function<void()> cb, const std::string name)
    :m_cb(cb)
    ,m_name(name)
{
    if (name.empty())
    {
        m_name = "UNKNOWN";
    }

    int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
    if (rt)
    {
        // SYLAR_LOG_ERROR(g_logger) << "pthread_create thread failed, rt = " << rt
            // << ", name = " << m_name;

        throw std::logic_error("pthread_create error");
    }

    m_semaphore.wait();
}
Thread::~Thread()
{
    if (m_thread)
    {
        pthread_detach(m_thread); // 也可以使用join
    }
}

void Thread::join()
{
    if (m_thread)
    {
        int rt = pthread_join(m_thread, nullptr);
        if (rt)
        {
            // SYLAR_LOG_ERROR(g_logger) << "pthread_join failed, rt = " << rt
                // << ", name = " << m_name;
            
            throw std::logic_error("pthread_join error");
        }
        m_thread = 0;
    }
}

void Thread::setName(std::string name)
{
    if (name.empty())
    {
        return;
    }
    if (t_thread)
    {
        t_thread->m_name = name;
    }
    t_thread_name = name;
}

Thread* Thread::GetThis()
{
    return t_thread;
}
const std::string& Thread::GetName()
{
    return t_thread_name;
}

void* Thread::run(void* arg)
{
    Thread* thread = (Thread*) arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = getThreadId();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb);

    thread->m_semaphore.notify();   // 确保线程创建完成后，该线程是已经跑起来的
    cb();

    SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "thread run end";
    return 0;
}
}