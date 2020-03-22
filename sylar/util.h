#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <vector>
#include <string>
#include <cxxabi.h>


namespace sylar
{

void Backtrace(std::vector<std::string>&bt, int size = 64, int skip = 1);

std::string Backtrace2String(int size = 64, int skip = 2, const std::string&prefix = "");


// 获取线程id
pid_t GetThreadId();

//　获取协程id
uint32_t GetFiberId();

/**
 * @brief 获取当前时间的毫秒
 */
uint64_t GetCurrentMS();

/**
 * @brief 获取当前时间的微秒
 */
uint64_t GetCurrentUS();
}

#endif