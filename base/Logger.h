#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H 

//其实这个Logger类只负责生成日志。
//日志写到哪？如何刷新，都是通过设置Logger的OutputFunc和FlushFunc来实现的。
//OutputFunc和FlushFunc由你自己的来实现
//OutputFunc和FlushFunc是全局的
#include "noncopyable.h"
#include "LogStream.h"

#include <string.h>

#include <functional>

class Logger : noncopyable
{
public:
    enum LogLevel 
    {
        INFO,
        TRACE,
        DEBUG,
        WARN,
        ERROR,
        FATAL,
        LEVEL_NUM,
    };

    class SourceFile
    {
    public:
        SourceFile(const char *pathname)
            : basename_(pathname),
              len_(sizeof(pathname) - 1)
        {
            const char *slash = strrchr(pathname, '/');
            if(slash)
            {
                basename_ = slash + 1;
                len_ -= static_cast<int>(slash - pathname) + 1;
            }
        }

        const char *basename() const { return basename_; }
        int len() const { return len_; }
    private:
        const char *basename_;
        int len_;
    };

    Logger(const SourceFile &file, int line, LogLevel level);
    Logger(const SourceFile &file, int line, LogLevel level, bool toAbort);
    ~Logger();


    LogStream &stream() { return stream_; }

    using OutputFunc = std::function<void(const char *, int)>;
    using FlushFunc = std::function<void()>;
    static void setOutputFunc(OutputFunc outputFunc);
    static void setFlushFunc(FlushFunc flushFunc);
    static void setLogLevel(LogLevel level);

private:
    LogStream stream_;
    LogLevel logLevel_;
    bool toAbort_;
};


extern Logger::LogLevel g_logLevel;

#define LOG_INFO if(g_logLevel <= Logger::INFO) Logger(__FILE__, __LINE__, Logger::INFO).stream()
#define LOG_TRACE if(g_logLevel <= Logger::TRACE) Logger(__FILE__, __LINE__, Logger::TRACE).stream()
#define LOG_DEBUG if(g_logLevel <= Logger::DEBUG) Logger(__FILE__, __LINE__, Logger::DEBUG).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR, false).stream()
#define LOG_FATA Logger(__FILE__, __LINE__, Logger::FATAL, true).stream()
#endif 