#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__

#include <pthread.h>
#include <string>
#include <functional>
#include <memory>

namespace sylar
{
class Thread
{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb, const std::string name);
    virtual ~Thread();

    pid_t getId() const {return m_id;}
    const std::string& getName() const {return m_name;}
    void setName(std::string name);

    void join();
    static Thread* GetThis();
    static const std::string& GetName();
private:
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    static void*run(void* arg);
private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;
};
}


#endif