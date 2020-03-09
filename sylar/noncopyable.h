#ifndef __SYLAR_NONCOPYABLE_H__
#define __SYLAR_NONCOPYABLE_H__

namespace sylar
{
class Noncopyable
{
public:
    Noncopyable() = default;
    virtual ~Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
};

#endif