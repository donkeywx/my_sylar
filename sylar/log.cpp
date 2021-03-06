#include "log.h"
#include "config.h"
#include <functional>
#include <map>
#include <iostream>

namespace sylar
{

const char* LogLevel::ToString(LogLevel::Level level)
{
    switch (level)
    {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;
    
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

LogLevel::Level LogLevel::FromString(const std::string& str) {
#define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LogLevel::UNKNOWN;
#undef XX
}

class MessageFormatItem: public LogFormatter::FormatItem
{
public:
    MessageFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getContent();
    }
};

class LevelFormatItem: public LogFormatter::FormatItem
{
public:
    LevelFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << LogLevel::ToString(level);
    }
};

class ElapseFormatItem: public LogFormatter::FormatItem
{
public:
    ElapseFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatItem {
public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getLogger()->getName();
    }
};

class ThreadIdFormatItem: public LogFormatter::FormatItem
{
public:
    ThreadIdFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getThreadId();
    }
};

class FiberIdFormatItem: public LogFormatter::FormatItem
{
public:
    FiberIdFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getFiberId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override 
    {
        os << event->getThreadName();
    }
};

class DateTimeFormatItem: public LogFormatter::FormatItem
{
public:
    DateTimeFormatItem(const std::string& format = "%Y:%m:%d %H:%M:%s")
        : m_format(format)
    {

    }
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        time_t time = event->getTime();
        struct tm tm;
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }
private:
    std::string m_format;
};

class FilenameFormatItem: public LogFormatter::FormatItem
{
public:
    FilenameFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getFile();
    }
};

class LineFormatItem: public LogFormatter::FormatItem
{
public:
    LineFormatItem(const std::string& str = "") {}
    virtual void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getLine();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string& str)
        :m_string(str) {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << m_string;
    }
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << "\t";
    }
private:
    std::string m_string;
};


LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
        const char* file, uint32_t line, uint64_t elapse,
        uint32_t threadId, uint32_t fiberId, uint64_t time,
        const std::string& threadName)
    : m_logger(logger), m_level(level), m_file(file),
        m_line(line), m_elapse(elapse), m_threadId(threadId),
        m_fiberId(fiberId), m_time(time), m_threadName(threadName)
{}

void LogEvent::format(const char* fmt, ...)
{
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}
void LogEvent::format(const char* fmt, va_list al)
{
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if(len != -1) 
    {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

LogEventWrap::LogEventWrap(LogEvent::ptr e)
    : m_event(e)
{

}

LogEventWrap::~LogEventWrap()
{
    // std::cout << "~" << std::endl;
    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

LogFormatter::LogFormatter(const std::string& pattern)
    :m_pattern(pattern)
{
    init();
}

std::ostream& LogFormatter::format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    for (auto& i : m_items)
    {
        i->format(ofs, logger, level, event);
    }
    return ofs;
}

// std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
// {
//     std::stringstream ss;
//     for (auto& i : m_items)
//     {
//         i->format(ss, logger, level, event);
//     }

//     return ss.str();
// }

// 日志格式解析
void LogFormatter::init() {
    //str, format, type
    std::vector<std::tuple<std::string, std::string, int> > vec;
    std::string nstr;
    for(size_t i = 0; i < m_pattern.size(); ++i) {
        if(m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }

        if((i + 1) < m_pattern.size()) {
            if(m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;
        while(n < m_pattern.size()) {
            if(!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                    && m_pattern[n] != '}')) {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if(fmt_status == 0) {
                if(m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    //std::cout << "*" << str << std::endl;
                    fmt_status = 1; //解析格式
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            } else if(fmt_status == 1) {
                if(m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    //std::cout << "#" << fmt << std::endl;
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if(n == m_pattern.size()) {
                if(str.empty()) {
                    str = m_pattern.substr(i + 1);
                }
            }
        }

        if(fmt_status == 0) {
            if(!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        } else if(fmt_status == 1) {
            std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        }
    }

    if(!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

        XX(m, MessageFormatItem),           //m:消息
        XX(p, LevelFormatItem),             //p:日志级别
        XX(r, ElapseFormatItem),            //r:累计毫秒数
        XX(c, NameFormatItem),              //c:日志名称
        XX(t, ThreadIdFormatItem),          //t:线程id
        XX(n, NewLineFormatItem),           //n:换行
        XX(d, DateTimeFormatItem),          //d:时间
        XX(f, FilenameFormatItem),          //f:文件名
        XX(l, LineFormatItem),              //l:行号
        XX(T, TabFormatItem),               //T:Tab
        XX(F, FiberIdFormatItem),           //F:协程id
        XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
    };

    for(auto& i : vec) {
        if(std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if(it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                m_error = true;
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }

        // std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") - (" << std::get<2>(i) << ")" << std::endl;
    }
    // std::cout << m_items.size() << std::endl;
}

LogLevel::Level LogAppender::getLevel()
{
    MutexType::ReadLock lock(m_mutex);
    return m_level;
}

void LogAppender::setLevel(LogLevel::Level level)
{
    MutexType::WriteLock lock(m_mutex);
    m_level = level;
}

bool LogAppender::hasFormatter()
{
    MutexType::ReadLock lock(m_mutex);
    return m_hasFormatter;
}

void LogAppender::setFormatter(LogFormatter::ptr val) 
{
    MutexType::WriteLock lock(m_mutex);
    m_formatter = val;
    if(m_formatter) 
    {
        m_hasFormatter = true;
    } 
    else 
    {
        m_hasFormatter = false;
    }
}

LogFormatter::ptr LogAppender::getFormatter()
{
    MutexType::ReadLock lock(m_mutex);
    return m_formatter;
}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    MutexType::ReadLock lock(m_mutex);
    if (level >= m_level)
    {
        m_formatter->format(std::cout, logger, level, event);
    }
}
std::string StdoutLogAppender::toYamlString() 
{
    MutexType::ReadLock lock(m_mutex);
    YAML::Node node;
    node["type"] = "StdoutLogAppender";
    if(m_level != LogLevel::UNKNOWN) 
    {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter) 
    {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}


FileLogAppender::FileLogAppender(const std::string& filename)
    :m_filename(filename)
{
    reopen();
}

FileLogAppender::~FileLogAppender()
{
    MutexType::ReadLock lock(m_mutex);
    if (m_filestream.is_open())
    {
        m_filestream.close();
    }
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    // MutexType::ReadLock lock(m_mutex);
    if (level >= m_level)
    {
        uint64_t now = event->getTime();
        if (now >= (m_lastTime + 3))
        {
            reopen();
            m_lastTime = now;
        }

        MutexType::WriteLock lock(m_mutex);
        if (!m_formatter->format(m_filestream, logger, level, event))
        {
            std::cout << "error" << std::endl;
        }
    }
}

std::string FileLogAppender::toYamlString()
{
    MutexType::ReadLock lock(m_mutex);
    YAML::Node node;
    node["type"] = "FileLogAppender";
    node["file"] = m_filename;
    if(m_level != LogLevel::UNKNOWN) 
    {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter) 
    {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

bool FileLogAppender::reopen()
{
    MutexType::WriteLock lock(m_mutex);
    if (m_filestream)
    {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);

    return !!m_filestream;
}


Logger::Logger(const std::string& name)
    :m_name(name),
    m_level(LogLevel::ERROR)
{
    m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
}

LogLevel::Level Logger::getLevel()
{
    MutexType::ReadLock lock(m_mutex);
    return m_level;
}

void Logger::setLevel(LogLevel::Level level)
{
    MutexType::WriteLock lock(m_mutex);
    m_level = level;
}

void Logger::addAppender(LogAppender::ptr appender)
{
    MutexType::WriteLock lock(m_mutex);
    if (!appender->hasFormatter())
    {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender)
{
    MutexType::WriteLock lock(m_mutex);
    for (auto it = m_appenders.begin();
        it != m_appenders.end(); it++)
    {
        if (*it == appender)
        {
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::clearAppender()
{
    MutexType::WriteLock lock(m_mutex);
    m_appenders.clear();
}

void Logger::setFormatter(LogFormatter::ptr val)
{
    MutexType::WriteLock lock(m_mutex);
    m_formatter = val;
    for (auto& appender : m_appenders)
    {
        // if (!appender->hasFormatter())
        // {
        appender->setFormatter(m_formatter);
        // }
    }
}

void Logger::setFormatter(const std::string& val)
{
    std::cout << "---" << val << std::endl;
    sylar::LogFormatter::ptr new_val(new sylar::LogFormatter(val));
    if(new_val->isError()) 
    {
        std::cout << "Logger setFormatter name=" << m_name
                  << " value=" << val << " invalid formatter"
                  << std::endl;
        return;
    }
    //m_formatter = new_val;
    setFormatter(new_val);
}

LogFormatter::ptr Logger::getFormatter()
{
    MutexType::ReadLock lock(m_mutex);
    return m_formatter;
}
Logger::ptr Logger::getRoot() 
{
    MutexType::ReadLock lock(m_mutex);
    return m_root;
}
void Logger::setRoot(Logger::ptr root) 
{
    MutexType::WriteLock lock(m_mutex);
    m_root = root;
}

std::string Logger::toYamlString() 
{
    MutexType::ReadLock lock(m_mutex);
    YAML::Node node;
    node["name"] = m_name;
    if(m_level != LogLevel::UNKNOWN) 
    {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_formatter) 
    {
        node["formatter"] = m_formatter->getPattern();
    }

    for(auto& i : m_appenders) 
    {
        node["appenders"].push_back(YAML::Load(i->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

void Logger::log(LogLevel::Level level, const LogEvent::ptr event)
{
    MutexType::ReadLock lock(m_mutex);
    if(level >= m_level) 
    {
        auto self = shared_from_this();

        if(!m_appenders.empty())
        {
            for(auto& i : m_appenders)
            {
                i->log(self, level, event);
            }
        }
        else if(m_root)
        {
            m_root->log(level, event);
        }
        else
        {
            std::cout << "There is not any appender in logger:" 
                << self->getName()
                << std::endl;
        }
        
    }
}

void Logger::debug(LogEvent::ptr event)
{
    log(LogLevel::DEBUG, event);
}
void Logger::error(LogEvent::ptr event)
{
    log(LogLevel::ERROR, event);
}
void Logger::info(LogEvent::ptr event)
{
    log(LogLevel::INFO, event);
}
void Logger::warn(LogEvent::ptr event)
{
    log(LogLevel::WARN, event);
}
void Logger::fatal(LogEvent::ptr event)
{
    log(LogLevel::FATAL, event);
}


LoggerManager::LoggerManager()
{
    m_root.reset(new Logger("root"));
    m_root->addAppender(LogAppender::ptr(new FileLogAppender("root.txt")));

    m_loggers[m_root->getName()] = m_root;

    init();
}

Logger::ptr LoggerManager::getLogger(const std::string& name) 
{
    {
        MutexType::ReadLock lock(m_mutex);
        auto it = m_loggers.find(name);
        if (it != m_loggers.end())
        {
            return it->second;
        }
    }

    {
        MutexType::WriteLock lock(m_mutex);
        Logger::ptr logger(new Logger(name));
        std::string logName = name;

        logger->setRoot(m_root);
        m_loggers[name] = logger;
        return logger;
    }
}

void LoggerManager::init()
{

}

std::string LoggerManager::toYamlString() 
{
    MutexType::ReadLock lock(m_mutex);
    YAML::Node node;
    for(auto& i : m_loggers)
    {
        node.push_back(YAML::Load(i.second->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

bool LogAppenderDefine::operator==(const LogAppenderDefine& other) const
{
    return m_type == other.m_type
        && m_level == other.m_level
        && m_formatter == other.m_formatter
        && m_file == other.m_file;
}

bool LogDefine::operator==(const LogDefine& other) const 
{
    return m_name == other.m_name
        && m_level == other.m_level
        && m_formatter == other.m_formatter;
}

bool LogDefine::operator<(const LogDefine& other) const 
{
    return m_name < other.m_name;
}

bool LogDefine::isValid() const 
{
    return !m_name.empty();
}

sylar::ConfigVar<std::set<LogDefine> >::ptr g_log_defines = 
    sylar::Config::Lookup("logs", std::set<LogDefine>(), "logs config");

template<>
class LexicalCast<std::string, LogDefine> {
public:
    LogDefine operator()(const std::string& v) {
        YAML::Node n = YAML::Load(v);
        LogDefine ld;
        if(!n["name"].IsDefined()) {
            std::cout << "log config error: name is null, " << n
                      << std::endl;
            throw std::logic_error("log config name is null");
        }
        ld.setName(n["name"].as<std::string>());
        ld.setLevel(LogLevel::FromString(n["level"].IsDefined() ? n["level"].as<std::string>() : ""));
        if(n["formatter"].IsDefined()) {
            ld.setFormatter(n["formatter"].as<std::string>());
        }

        if(n["appenders"].IsDefined()) {
            //std::cout << "==" << ld.name << " = " << n["appenders"].size() << std::endl;
            for(size_t x = 0; x < n["appenders"].size(); ++x) {
                auto a = n["appenders"][x];
                if(!a["type"].IsDefined()) {
                    std::cout << "log config error: appender type is null, " << a
                              << std::endl;
                    continue;
                }
                std::string type = a["type"].as<std::string>();
                LogAppenderDefine lad;
                if(type == "FileLogAppender") {
                    lad.setType(AppenderType::Type::FILEAPPENDER);
                    if(!a["file"].IsDefined()) {
                        std::cout << "log config error: fileappender file is null, " << a
                              << std::endl;
                        continue;
                    }
                    lad.setFile(a["file"].as<std::string>());
                    if(a["formatter"].IsDefined()) {
                        lad.setFormatter(a["formatter"].as<std::string>());
                    }
                } else if(type == "StdoutLogAppender") {
                    lad.setType(AppenderType::Type::STDCOUTAPPENDER);
                    if(a["formatter"].IsDefined()) {
                        lad.setFormatter(a["formatter"].as<std::string>());
                    }
                } else {
                    std::cout << "log config error: appender type is invalid, " << a
                              << std::endl;
                    continue;
                }

                ld.addLogAppenderDefine(lad);
            }
        }
        return ld;
    }
};

template<>
class LexicalCast<LogDefine, std::string> {
public:
    std::string operator()(const LogDefine& i) {
        YAML::Node n;
        n["name"] = i.getName();
        if(i.getLevel() != LogLevel::UNKNOWN) {
            n["level"] = LogLevel::ToString(i.getLevel());
        }
        if(!i.getFormatter().empty()) {
            n["formatter"] = i.getFormatter();
        }

        for(auto& a : i.getAppenders()) {
            YAML::Node na;
            if(a.getType() == AppenderType::Type::FILEAPPENDER) {
                na["type"] = "FileLogAppender";
                na["file"] = a.getFile();
            } else if(a.getType() == AppenderType::Type::STDCOUTAPPENDER) {
                na["type"] = "StdoutLogAppender";
            }
            if(a.getLevel() != LogLevel::UNKNOWN) {
                na["level"] = LogLevel::ToString(a.getLevel());
            }

            if(!a.getFormatter().empty()) {
                na["formatter"] = a.getFormatter();
            }

            n["appenders"].push_back(na);
        }
        std::stringstream ss;
        ss << n;
        return ss.str();
    }
};

LogIniter::LogIniter()
{
    std::function<void (const std::set<LogDefine>&, const std::set<LogDefine>&)> cb = resetLogger;
    g_log_defines->addListener(cb);
}

void LogIniter::resetLogger(const std::set<LogDefine>& oldValue,
            const std::set<LogDefine>& newValue)
{
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "on_logger_conf_changed";
    for (auto& newLogDefine : newValue)
    {
        auto oldLogDefine = oldValue.find(newLogDefine);
        sylar::Logger::ptr logger;
        if (oldValue.end() == oldLogDefine)
        {
            // 新增logger
            logger = SYLAR_LOG_NAME(newLogDefine.getName());
        }
        else
        {
            // old中有， new中也有，但是不相同，就是修改
            if (!(newLogDefine == *oldLogDefine))
            {
                logger = SYLAR_LOG_NAME(newLogDefine.getName());
            }
            else
            {
                continue;
            }
        }
        
        logger->setLevel(newLogDefine.getLevel());
        if (!newLogDefine.getFormatter().empty())
        {
            logger->setFormatter(newLogDefine.getFormatter());
            std::cout << newLogDefine.getFormatter() << std::endl;
        }

        resetLoggerAppender(logger, newLogDefine);
    }
    for(auto& oldLogDefine : oldValue) 
    {
        auto it = newValue.find(oldLogDefine);
        if(it == newValue.end()) 
        {
            //删除logger
            auto logger = SYLAR_LOG_NAME(oldLogDefine.getName());
            logger->setLevel((LogLevel::Level)0);
            logger->clearAppender();
        }
    }
}

void LogIniter::resetLoggerAppender(Logger::ptr logger, const LogDefine& newLogDefine)
{
    logger->clearAppender();
    std::vector<LogAppenderDefine> newAppenders= newLogDefine.getAppenders();
    for (auto appender : newAppenders)
    {
        sylar::LogAppender::ptr newAppender;
        if(AppenderType::Type::FILEAPPENDER == appender.getType())
        {
            newAppender.reset(new FileLogAppender(appender.getFile()));
        } 
        else if(AppenderType::Type::STDCOUTAPPENDER ==  appender.getType()) 
        {
            // if(!sylar::EnvMgr::GetInstance()->has("d")) {
            newAppender.reset(new StdoutLogAppender);
            // } else {
                // continue;
            // }
        }
        newAppender->setLevel(appender.getLevel());
        if(!appender.getFormatter().empty()) 
        {
            LogFormatter::ptr fmt(new LogFormatter(appender.getFormatter()));
            if(!fmt->isError()) 
            {
                newAppender->setFormatter(fmt);
            } else 
            {
                std::cout << "log.name=" << newLogDefine.getName() << " appender type=" << (int)appender.getType()
                        << " formatter=" << appender.getFormatter() << " is invalid" << std::endl;
            }
        }
        logger->addAppender(newAppender);
    }
}
// 确保__log_init在程序执行前完成
static LogIniter __log_init;
}