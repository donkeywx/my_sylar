#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <memory>
#include <sstream>
#include <list>
#include <vector>
#include <fstream>

namespace sylar
{

class Logger;

// 日志事件
class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

    const char* getFile() const {return m_file;}
    uint32_t getLine() const {return m_line;}
    int32_t getElapse() const {return m_elapse;}
    int32_t getThreadId() const {return m_threadId;}
    int32_t getFiberId() const {return m_fiberId;}
    int64_t getTime() const {return m_time;}
    std::string getContent() const {return m_content;}
    std::string getThreadName() const {return m_threadName;}
private:
    const char* m_file = nullptr;   // 文件名
    uint32_t m_line = 0;            // 行号
    uint32_t m_elapse = 0;          // 程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;        // 线程id
    std::string m_threadName;       // 线程名
    uint32_t m_fiberId = 0;         // 协程号id
    uint64_t m_time;                // 时间错
    std::string  m_content;
};

// 日志级别
class LogLevel
{
public:
    enum Level
    {
        UNKNOWN = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };

    static const char* ToString(LogLevel::Level level);
};

// 日志格式
class LogFormatter
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem
    {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
    void init();
private:
    bool m_error = false;
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

// 日志输出地
class LogAppender
{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr formatter){m_formatter = formatter;}
    LogFormatter::ptr getFormatter()const {return m_formatter;}
protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};


// 日志器
class Logger: public std::enable_shared_from_this<Logger>
{
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");

    void log(LogLevel::Level level, const LogEvent::ptr event);
    
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLogLevel()const{return m_level;}
    void setLogLevel(LogLevel::Level level){m_level = level;}
    const std::string& getName() const { return m_name;}

    void debug(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_logAppenders;
};

// 输出到控制台的Appender
class StdoutLogAppender: public LogAppender
{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
};

// 输出到文件的Appender
class FileLogAppender: public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

    // 重新打开文件，返回true
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

}
#endif