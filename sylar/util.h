#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <vector>
#include <string>
#include <stdio.h>
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

class FSUtil {
public:
    static void ListAllFile(std::vector<std::string>& files
                            ,const std::string& path
                            ,const std::string& subfix);
    static bool Mkdir(const std::string& dirname);
    static bool IsRunningPidfile(const std::string& pidfile);
    static bool Rm(const std::string& path);
    static bool Mv(const std::string& from, const std::string& to);
    static bool Realpath(const std::string& path, std::string& rpath);
    static bool Symlink(const std::string& frm, const std::string& to);
    static bool Unlink(const std::string& filename, bool exist = false);
    static std::string Dirname(const std::string& filename);
    static std::string Basename(const std::string& filename);
    // static bool OpenForRead(std::ifstream& ifs, const std::string& filename
    //                 ,std::ios_base::openmode mode);
    // static bool OpenForWrite(std::ofstream& ofs, const std::string& filename
    //                 ,std::ios_base::openmode mode);
};
}

#endif