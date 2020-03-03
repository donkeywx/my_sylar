#ifndef __SYLAR_SINGLETON_H__
#define __SYLAR_SINGLETON_H__

#include <memory>

namespace  sylar
{
namespace
{
template<class T, class X, int N>
T& getInstanceX()
{
    static T v;
    return v;
}

template<class T, class X, int N>
std::shared_ptr<T> getInstancePtr()
{
    static std::shared_ptr<T> v(new T);
    return v;
} 
}

/**
 * @brief 单例模式封装类
 * @details T 类型
 *          X 为了创造多个实例对应的Tag
 *          N 同一个Tag创造多个实例索引
 */
template<class T, class X = void, int N = 0>
class Singleton
{
public:
    static T* getInstance()
    {
        static T v;
        return &v;
    }
};

template<class T, class X = void, int N = 0>
class Siingleton
{
public:
    static std::shared_ptr<T> getInstance() 
    {
        static std::shared_ptr<T> v(new T);
        return v;
    }
};
}


#endif