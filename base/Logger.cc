#include "Logger.h"
#include "Timestamp.h"
#include <errno.h>
#include <stdlib.h>

Logger::OutputFunc g_outputFunc;
Logger::FlushFunc g_flushFunc;
Logger::LogLevel g_logLevel;

void Logger::setOutputFunc(OutputFunc outputFunc)
{
    g_outputFunc = outputFunc;
}

void Logger::setFlushFunc(FlushFunc flushFunc)
{
    g_flushFunc = flushFunc;
}

void Logger::setLogLevel(LogLevel level)
{
    g_logLevel = level;
}

const char *g_logLevelName[Logger::LEVEL_NUM] = {
    "INFO",
    "TRACE",
    "DEBUG",
    "WARN",
    "ERROR",
    "FATAL",
};

//时间 + 日志等级 + 日志信息 + 文件名 + 行号
Logger::Logger(const SourceFile &file, int line, LogLevel level)
    : stream_(Timestamp::now(), g_logLevelName[level], file.basename(), line, 0),
      logLevel_(level),
      toAbort_(false)
{

}


Logger::Logger(const SourceFile &file, int line, LogLevel level, bool toAbort)
    : stream_(Timestamp::now(), g_logLevelName[level], file.basename(), line, errno),
      logLevel_(level),
      toAbort_(toAbort)
{

}

Logger::~Logger()
{
    stream_.finish();
    const LogStream::Buffer &buffer = stream_.data();
    g_outputFunc(buffer.data(), buffer.size());
    if(toAbort_)
    {
        if(g_flushFunc)
            g_flushFunc();
        abort();
    }
}