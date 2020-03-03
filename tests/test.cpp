#include <iostream>
#include "../sylar/log.h"

int main(int argc, char** argv) {

    sylar::Logger::ptr logger(new sylar::Logger());
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender()));
    // sylar::LogEvent::ptr event(new sylar::LogEvent(logger, sylar::LogLevel::ERROR, __FILE__, __LINE__, 0, 
    // sylar::getThreadId(),sylar::getFiberId(), time(0), "thread_name"));

    // event->getSS() << "hello fuck";
    // logger->log(sylar::LogLevel::ERROR, event);
    // std::cout << "hello sylar log" << std::endl;
    // sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    // sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    // file_appender->setFormatter(fmt);
    // file_appender->setLevel(sylar::LogLevel::INFO);

    // logger->addAppender(file_appender);


    SYLAR_LOG_INFO(logger) << "test macro";
    SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = sylar::LoggerMgr::getInstance()->getLogger("xx");
    SYLAR_LOG_ERROR(l) << "xxx";

    auto root = sylar::LoggerMgr::getInstance()->getRoot();
    SYLAR_LOG_ERROR(root) << "root";
    return 0;
}
