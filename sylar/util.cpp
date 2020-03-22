#include "util.h"
#include "log.h"
#include "fiber.h"
#include <sys/stat.h>
#include <execinfo.h>
#include <sys/time.h>


namespace sylar
{

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

static std::string demangle(const char* str) 
{
    size_t size = 0;
    int status = 0;
    std::string rt;
    rt.resize(256);
    if(1 == sscanf(str, "%*[^(]%*[^_]%255[^)+]", &rt[0])) 
    {
        char* v = abi::__cxa_demangle(&rt[0], nullptr, &size, &status);
        if(v) 
        {
            std::string result(v);
            free(v);
            return result;
        }
    }
    if(1 == sscanf(str, "%255s", &rt[0]))
    {
        return rt;
    }
    return str;
}

void Backtrace(std::vector<std::string>&bt, int size, int skip)
{
    void** array = (void**)malloc((sizeof(void*) * size));
    if (nullptr == array)
    {
        SYLAR_LOG_ERROR(g_logger) << "malloc error";
        return;
    }

    size_t s = ::backtrace(array, size);    // 返回实际的堆栈层数
    char** strings = backtrace_symbols(array, s);
    do
    {
        if (nullptr == strings)
        {
            SYLAR_LOG_ERROR(g_logger) << "backtrace_symbols error";
            break;
        }

        for (size_t i = skip; i < s; i++)
        {
            bt.push_back(demangle(strings[i])); // 格式化字符串
        }
    } while(0);

    if (nullptr != strings)
    {
        free(strings);
    }
    if (nullptr != array)
    {
        free(array);
    }
}

std::string Backtrace2String(int size, int skip, const std::string&prefix)
{
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < bt.size(); i++)
    {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

pid_t GetThreadId()
{
    return syscall(SYS_gettid);
}

uint32_t GetFiberId()
{
    return sylar::Fiber::GetFiberId();
}

uint64_t GetCurrentMS() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000ul  + tv.tv_usec / 1000;
}

uint64_t GetCurrentUS() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 * 1000ul  + tv.tv_usec;
}

static int __lstat(const char* file, struct stat* st = nullptr) {
    struct stat lst;
    int ret = lstat(file, &lst);
    if(st) {
        *st = lst;
    }
    return ret;
}

bool FSUtil::Unlink(const std::string& filename, bool exist) {
    if(!exist && __lstat(filename.c_str())) {
        return true;
    }
    return ::unlink(filename.c_str()) == 0;
}

}