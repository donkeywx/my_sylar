#include "../sylar.h"
#include <iostream>
static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

static const int NTHREADS = 10;

// 本系统日志 单线程 100万 8.7s 
// 本系统日志 10线程各 50万 161.143 
void* printfLog(void* arg)
{
    
    // # AsyncAppender
    for (int i = 0; i < 500/NTHREADS; i++)
    {
        for (int j = 0 ; j < 10000; j++)
        {
            SYLAR_LOG_ERROR(g_logger) << "fsfdsfasfafafasfffffsfasfs efsfasf wr fa efafarfrf";
        }
    }

    return nullptr;
}


int main()
{
    time_t start, end;
    start = clock();
    

    pthread_t threads[NTHREADS];
    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_create(&threads[i], nullptr, printfLog, nullptr);
    }


    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    end = clock();
    std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
    std::cout<<"success"<< std::endl;
    return 0;
}


// #include <iostream>
// #include "../sylar/log.h"

// int main(int argc, char** argv) {

//     // logger需要添加appender，如果appender没有指定formatter，使用logger默认的formatter
//     // logger在初始化的时候，有默认的formatter，也可以指定formatter.最好在添加appender之前修改这个属性
//     // 因为修改logger默认的formatter会连带修改所有的appender的formatter

//     sylar::Logger::ptr logger(new sylar::Logger("test"));
//     logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender()));

//     // 没打印一句日志，都会产生一个event对象。
//     sylar::LogEvent::ptr event(new sylar::LogEvent(logger, sylar::LogLevel::ERROR, __FILE__, __LINE__, 0, 
//     sylar::GetThreadId(),sylar::GetFiberId(), time(0), "thread_name"));

//     event->getSS() << "hello fuck";
//     // event->format("adsf%d", 1);
//     logger->log(sylar::LogLevel::ERROR, event);

//     sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
//     sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
//     file_appender->setFormatter(fmt);
//     file_appender->setLevel(sylar::LogLevel::INFO);

//     logger->addAppender(file_appender);


//     SYLAR_LOG_INFO(logger) << "test macro";
//     SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

//     // SYLAR_LOG_NAME(name)
//     auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
//     SYLAR_LOG_ERROR(l) << "xxx";

//     // SYLAR_LOG_ROOT();
//     auto root = sylar::LoggerMgr::GetInstance()->getRoot();
//     SYLAR_LOG_ERROR(root) << "root";
//     return 0;
// }
