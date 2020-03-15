#ifndef __SYLAR_MUTEX_H__
#define __SYLAR_MUTEX_H__

#include "noncopyable.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

namespace sylar
{
class Semaphore: public Noncopyable
{
public:
    Semaphore(uint32_t count = 0);

    virtual ~Semaphore();

    void wait();

    void notify();
private:
    sem_t m_semaphore;
};

template<class T>
class ScopedLockImpl
{
public:
    ScopedLockImpl(T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~ScopedLockImpl()
    {
        m_mutex.unlock();
    }

    void unlock()
    {
        m_mutex.unlock();
    }
private:
    T& m_mutex;
};

template<class T>
struct ReadScopedLockImpl {
public:
    ReadScopedLockImpl(T& redlock)
        : m_rdlock(redlock)
    {
        m_rdlock.rdlock();
    }

    ~ReadScopedLockImpl()
    {
        m_rdlock.unlock();
    }

    void unlock()
    {
        m_rdlock.unlock();
    }
private:

    T& m_rdlock;
};

/**
 * @brief 局部写锁模板实现
 */
template<class T>
struct WriteScopedLockImpl {
public:
    WriteScopedLockImpl(T& wrlock)
        : m_wrlock(wrlock)
    {
        m_wrlock.wrlock();
    }

    ~WriteScopedLockImpl()
    {
        m_wrlock.unlock();
    }

    void unlock()
    {
        m_wrlock.unlock();
    }
private:
    T& m_wrlock;
};

class SpinLock: public Noncopyable
{
public:
    typedef ScopedLockImpl<SpinLock> Lock;

    SpinLock(){pthread_spin_init(&spin, 0);}

    ~SpinLock(){pthread_spin_destroy(&spin);}

    void lock(){pthread_spin_lock(&spin);}

    void unlock(){pthread_spin_unlock(&spin);}

private:
    pthread_spinlock_t spin;
};

class Mutex: public Noncopyable
{
public:
    typedef ScopedLockImpl<Mutex> Lock;

    Mutex()
    {   
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_mutex, &attr);
    }

    virtual ~Mutex(){pthread_mutex_destroy(&m_mutex);}

    void lock(){pthread_mutex_lock(&m_mutex);}

    void unlock(){pthread_mutex_unlock(&m_mutex);}
private:
    pthread_mutex_t m_mutex;
    pthread_mutexattr_t attr;
};

class RWLock: public Noncopyable
{
public:
    /// 局部读锁
    typedef ReadScopedLockImpl<RWLock> ReadLock;

    /// 局部写锁
    typedef WriteScopedLockImpl<RWLock> WriteLock;

    RWLock(){pthread_rwlock_init(&m_rwlock, nullptr);}

    virtual ~RWLock(){pthread_rwlock_destroy(&m_rwlock);}

    void wrlock(){pthread_rwlock_wrlock(&m_rwlock);}

    void rdlock(){pthread_rwlock_rdlock(&m_rwlock);}

    void unlock(){pthread_rwlock_unlock(&m_rwlock);}
private:
    pthread_rwlock_t m_rwlock;
};

}
#endif