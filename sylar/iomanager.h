#ifndef __SYLAR_IOMANAGER_H__
#define __SYLAR_IOMANAGER_H__

#include "scheduler.h"

namespace sylar
{

class IOManager: public Scheduler
{
public:
    typedef std::shared_ptr<IOManager> ptr;
    typedef RWLock MutexType;

    /**
     * IO事件 
     */
    enum Event
    {
        // 无事件
        NONE = 0x00,
        // 读事件
        READ = 0X01,
        // 写事件
        WRITE = 0X04
    };
private:
    // socket事件上下文类
    struct FdContext
    {
        typedef Mutex MutexType;
        
        // 事件上下文
        struct EventContext
        {
            // 事件执行的调度器
            Scheduler* scheduler = nullptr;
            // 事件协程
            Fiber::ptr fiber;
            // 事件的回调函数
            std::function<void()> cb;        
        };

        EventContext& getEventContext(Event evnet);

        void resetEventContext(EventContext& ctx);

        void triggerEvent(Event event);

        EventContext read;
        EventContext write;

        int fd = 0;
        Event events = NONE;
        MutexType mutex;
    };
public:
    // 参数：线程数，是否将调用线程包含进去，调度器的名称
    IOManager(size_t threads = 1, bool use_caller = true, const std::string& name = "");
    ~IOManager();

    int addEvent(int fd, Event event, std::function<void()> cb = nullptr);
    bool delEvent(int fd, Event event);
    // 取消某个fd的某个事件
    bool cancelEvent(int fd, Event event);
    // 取消某个fd的所有事件
    bool calcelAll(int fd);
    static IOManager* GetThis();

protected:
    void tickle() override;
    bool stopping() override;
    void idle() override;
    // void onTimerIndertedAtFront();

    void contextResize(size_t size);
    bool stopping(uint64_t& timeout);
};
}

#endif