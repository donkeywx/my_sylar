#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include "util.h"
#include "mutex.h"
#include "thread.h"
#include "singleton.h"
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include <vector>
#include <fstream>
#include <time.h>
#include <map>
#include <set>

// 使用流方式输出日志
// 大致流程就是获取event的输出流，然后在LogEventWrap析构的时候输出内容
#define SYLAR_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr (new sylar::LogEvent(logger, level, \
                            __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                            sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getSS()

#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

// 格式化输出日志
// 和SYLAR_LOG_LEVEL差不多，但是多了format解析的过程，具体日志内容存储在m_ss中
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                        sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__) // __VA_ARGS__ 传递上层函数的可变参数
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...)   SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...)   SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...)   SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

// 获取主日志
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

// 根据名称获取对应的日志器
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)

namespace sylar
{

class Logger;


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

    static LogLevel::Level FromString(const std::string& str);
};
class AppenderType
{
public:
    enum Type
    {
        UNKNOWN = 0,
        STDCOUTAPPENDER = 1,
        FILEAPPENDER = 2
    };
};
// 日志事件
// 该类最重要的事情就是记录日志内容的所有属性，以及具体日志内容
class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    /**
     * @brief 构造函数
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] file 文件名
     * @param[in] line 文件行号
     * @param[in] elapse 程序启动依赖的耗时(毫秒)
     * @param[in] thread_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time 日志事件(秒)
     * @param[in] thread_name 线程名称
     */
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
        const char* file, uint32_t line, uint64_t elapse,
        uint32_t threadId, uint32_t fiberId, uint64_t time,
        const std::string& threadName);

    const char* getFile() const {return m_file;}
    uint32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    std::string getThreadName() const {return m_threadName;}
    uint32_t getFiberId() const {return m_fiberId;}
    uint64_t getTime() const {return m_time;}
    std::stringstream& getSS() {return m_ss;}
    std::string getContent() const{return m_ss.str();}
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
    LogLevel::Level getLevel() const {return m_level;}

    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private:
    std::shared_ptr<Logger> m_logger;// 日志器
    LogLevel::Level m_level;        // 日志等级
    const char* m_file = nullptr;   // 文件名
    uint32_t m_line = 0;            // 行号
    uint32_t m_elapse = 0;          // 程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;        // 线程id
    uint32_t m_fiberId = 0;         // 协程号id
    uint64_t m_time = 0;            // 时间错
    std::string m_threadName;       // 线程名
    std::stringstream m_ss;         // 日志内容流
};

class LogEventWrap
{
public:
    LogEventWrap(LogEvent::ptr e);

    virtual ~LogEventWrap();

    // 获取日志事件
    LogEvent::ptr getEvent(){return m_event;}

    // 获取日志内容流
    std::stringstream& getSS() {return m_event->getSS();}
private:
    LogEvent::ptr m_event;
};

// 日志格式
class LogFormatter
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    // std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem
    {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
    void init();

    bool isError() const {return m_error;}

    const std::string getPattern() const {return m_pattern;}
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
    typedef RWLock MutexType;

    virtual ~LogAppender(){}

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    virtual std::string toYamlString() = 0;

    LogLevel::Level getLevel();
    void setLevel(LogLevel::Level level);

    bool hasFormatter();
    void setFormatter(LogFormatter::ptr formatter);
    LogFormatter::ptr getFormatter();
protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    bool m_hasFormatter = false;
    LogFormatter::ptr m_formatter;
    MutexType m_mutex;
};

// 输出到控制台的Appender
class StdoutLogAppender: public LogAppender
{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;
};

// 输出到文件的Appender
class FileLogAppender: public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    FileLogAppender(const std::string& filename);
    virtual ~FileLogAppender();
    
    
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

    std::string toYamlString() override;
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime = 0;    // 上次打开的时间
};


// 日志器
class Logger: public std::enable_shared_from_this<Logger>
{
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef RWLock MutexType;

    Logger(const std::string& name = "root");

    void log(LogLevel::Level level, const LogEvent::ptr event);
    
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    const std::string& getName() const { return m_name;}

    LogLevel::Level getLevel();
    void setLevel(LogLevel::Level level);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppender();

    void setFormatter(LogFormatter::ptr val);
    void setFormatter(const std::string& val);
    LogFormatter::ptr getFormatter();

    Logger::ptr getRoot();
    void setRoot(Logger::ptr root);

    std::string toYamlString();
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_appenders;
    LogFormatter::ptr m_formatter;
    MutexType m_mutex;
    Logger::ptr m_root; // 在不指定任何的appender的时候，使用root日志器
};

class LoggerManager
{
public:
    typedef RWLock MutexType;
    LoggerManager();

    Logger::ptr getLogger(const std::string& name);

    void init();

    Logger::ptr getRoot() const {return m_root;}

    std::string toYamlString();
private:

    std::map<std::string, Logger::ptr> m_loggers;
    
    Logger::ptr m_root;

    MutexType m_mutex;
};
/// 日志器管理类单例模式
typedef sylar::Singleton<LoggerManager> LoggerMgr;


class  LogAppenderDefine
{
public:

    bool operator==(const LogAppenderDefine& other) const;

    AppenderType::Type getType() const{return m_type;}
    void setType(AppenderType::Type type){m_type = type;}
    LogLevel::Level getLevel() const{return m_level;}
    void setLevel(LogLevel::Level level){m_level = level;}
    std::string getFormatter() const{return m_formatter;}
    void setFormatter(std::string formatter){m_formatter = formatter;}
    std::string getFile() const {return m_file;}
    void setFile(std::string file){m_file = file;}
private:
    AppenderType::Type m_type = AppenderType::Type::UNKNOWN;
    LogLevel::Level m_level = LogLevel::UNKNOWN;
    std::string m_formatter;
    std::string m_file;
};

class LogDefine
{
public:
    bool operator==(const LogDefine& other) const;

    bool operator<(const LogDefine& other) const;

    bool isValid() const;

    std::string getName() const {return m_name;}
    void setName(std::string name){m_name = name;}
    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level level) {m_level = level;}
    std::string getFormatter() const {return m_formatter;}
    void setFormatter(std::string formatter){m_formatter = formatter;}
    // todo: 返回一个引用
    const std::vector<LogAppenderDefine>& getAppenders() const {return m_appenders;}
    void addLogAppenderDefine(LogAppenderDefine& ad) {m_appenders.push_back(ad);}
private:
    std::string m_name;
    LogLevel::Level m_level = LogLevel::UNKNOWN;
    std::string m_formatter;
    std::vector<LogAppenderDefine> m_appenders;
};

class LogIniter
{
public:
    LogIniter();
private:
    static void resetLogger(const std::set<LogDefine>& oldValue,
            const std::set<LogDefine>& newValue);
    static void resetLoggerAppender(Logger::ptr logger, const LogDefine& newLogDefine);
};

}

#endif