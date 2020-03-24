#include "timer.h"
#include "util.h"

namespace sylar {

bool Timer::Comparator::operator()(const Timer::ptr& lhs
                        ,const Timer::ptr& rhs) const
{
    if(!lhs && !rhs) {
        return false;
    }
    if(!lhs) {
        return true;
    }
    if(!rhs) {
        return false;
    }
    if(lhs->m_next < rhs->m_next) {
        return true;
    }
    if(rhs->m_next < lhs->m_next) {
        return false;
    }
    return lhs.get() < rhs.get();
}


Timer::Timer(uint64_t ms, std::function<void()> cb,
             bool recurring, TimerManager* manager)
    :m_recurring(recurring)
    ,m_ms(ms)
    ,m_cb(cb)
    ,m_manager(manager)
{
    m_next = sylar::GetCurrentMS() + m_ms;
}

Timer::Timer(uint64_t next)
    :m_next(next)
{
}

bool Timer::cancel()
{
    if(m_cb)
    {
        m_cb = nullptr;
        m_manager->deleteTimer(shared_from_this());
        return true;
    }
    return false;
}

bool Timer::refresh() 
{
    if(!m_cb) 
    {
        return false;
    }
    m_manager->deleteTimer(shared_from_this());

    m_next = sylar::GetCurrentMS() + m_ms;
    m_manager->addTimer(shared_from_this());
    return true;
}

bool Timer::reset(uint64_t ms, bool from_now) {
    if(ms == m_ms && !from_now) {
        return true;
    }
    m_manager->deleteTimer(shared_from_this());

    uint64_t start = 0;
    if(from_now)
    {
        start = sylar::GetCurrentMS();
    }
    else
    {
        start = m_next - m_ms;
    }
    m_ms = ms;
    m_next = start + m_ms;
    m_manager->addTimer(shared_from_this());
    return true;

}

TimerManager::TimerManager()
{
    m_previouseTime = sylar::GetCurrentMS();
}

TimerManager::~TimerManager()
{
}

Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void()> cb
                                  ,bool recurring) 
{
    Timer::ptr timer(new Timer(ms, cb, recurring, this));
    addTimer(timer);
    return timer;
}

static void OnTimer(std::weak_ptr<void> weak_cond, std::function<void()> cb)
{
    std::shared_ptr<void> tmp = weak_cond.lock();
    if(tmp) {
        cb();
    }
}

Timer::ptr TimerManager::addConditionTimer(uint64_t ms, std::function<void()> cb
                                    ,std::weak_ptr<void> weak_cond
                                    ,bool recurring) {
    return addTimer(ms, std::bind(&OnTimer, weak_cond, cb), recurring);
}

bool TimerManager::deleteTimer(Timer::ptr val)
{
    TimerManager::RWMutexType::Lock loc(m_mutex);
    auto it = m_timers.find(val);
    if(it == m_timers.end())
    {
        return false;
    }
    m_timers.erase(it);

    return true;
}

uint64_t TimerManager::getNextTimer() {
    RWMutexType::Lock lock(m_mutex);
    m_tickled = false;
    if(m_timers.empty()) {
        return ~0ull;
    }

    const Timer::ptr& next = *m_timers.begin();
    uint64_t now_ms = sylar::GetCurrentMS();
    if(now_ms >= next->getNext()) {
        return 0;
    } else {
        return next->getNext() - now_ms;
    }
}

void TimerManager::listExpiredCb(std::vector<std::function<void()> >& cbs) {
    uint64_t now_ms = sylar::GetCurrentMS();
    std::vector<Timer::ptr> expired;
    {
        RWMutexType::Lock lock(m_mutex);
        if(m_timers.empty()) {
            return;
        }
    }
    RWMutexType::Lock lock(m_mutex);
    if(m_timers.empty()) {
        return;
    }
    bool rollover = detectClockRollover(now_ms);
    if(!rollover && ((*m_timers.begin())->getNext() > now_ms)) {
        return;
    }

    Timer::ptr now_timer(new Timer(now_ms));
    auto it = rollover ? m_timers.end() : m_timers.lower_bound(now_timer);
    while(it != m_timers.end() && (*it)->getNext() == now_ms) {
        ++it;
    }
    expired.insert(expired.begin(), m_timers.begin(), it);
    m_timers.erase(m_timers.begin(), it);
    cbs.reserve(expired.size());

    for(auto& timer : expired) {
        cbs.push_back(timer->getCb());
        if(timer->getRecurring()) {
            timer->setNext(now_ms + timer->getMs());
            m_timers.insert(timer);
        } else {
            timer->setCb(nullptr);
        }
    }
}

void TimerManager::addTimer(Timer::ptr val)
{
    TimerManager::RWMutexType::Lock lock(m_mutex);
    auto it = m_timers.insert(val).first;
    bool at_front = (it == m_timers.begin()) && !m_tickled;
    if(at_front)
    {
        m_tickled = true;
    }
    if(at_front)
    {
        onTimerInsertedAtFront();
    }
}

bool TimerManager::detectClockRollover(uint64_t now_ms) {
    bool rollover = false;
    if(now_ms < m_previouseTime &&
            now_ms < (m_previouseTime - 60 * 60 * 1000)) {
        rollover = true;
    }
    m_previouseTime = now_ms;
    return rollover;
}

bool TimerManager::hasTimer() {
    RWMutexType::Lock lock(m_mutex);
    return !m_timers.empty();
}

}
