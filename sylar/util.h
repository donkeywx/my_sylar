#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

namespace sylar
{

// 获取线程id
pid_t getThreadId();

//　获取协程id
uint32_t getFiberId();
}

#endif