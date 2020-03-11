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
pid_t getThreadId();

//　获取协程id
uint32_t getFiberId();
}

#endif