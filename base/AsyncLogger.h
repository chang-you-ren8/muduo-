//日志线程是消费者，负责将日志写到磁盘中
//其他线程是生产者线程。AsyncLogger对外只提供append函数。
//使用基于对象的设计方法,因此AsyncLogger包含一个线程对象，这个线程就是日志线程，它调用AsyncLogger的threadFunc函数,来向
//磁盘写入数据
#ifndef BASE_ASYNCLOGGER_H
#define BASE_ASYNCLOGGER_H 

#include "MutexLock.h"
#include "MutexLockGuard.h"
#include "Condition.h"
#include "Thread.h"
#include "noncopyable.h"
#include "FixedBuffer.h"

#include <unistd.h>

#include <vector>
#include <memory>
#include <string>

class AsyncLogger : noncopyable
{
public:
    AsyncLogger(const std::string &basename,
                off_t rollSize = 1024 * 1024, 
                time_t flushInterval = 3);
    ~AsyncLogger();

    void append(const char *msg, int len);
    void start();
    void stop();

private:
    const std::string basename_;
    const off_t rollSize_;
    const time_t flushInterval_;
    bool running_;

    MutexLock mutex_;
    Condition cond_;
    Thread thread_;
    
    using Buffer = BigFixedBuffer;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

    BufferPtr curBuffer_;
    BufferPtr nxtBuffer_;
    BufferVector buffers_;

    void threadFunc();
};

#endif 