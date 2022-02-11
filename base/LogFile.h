#ifndef BASE_LOGFILE_H
#define BASE_LOGFILE_H

#include "noncopyable.h"
#include "AppendFile.h"
#include "MutexLock.h"

#include <time.h>
#include <unistd.h>

#include <memory>
#include <string>

//满足一笑条件之一就滚动日志
//1.文件中大小超过rollByteSize_
//2.过了一天
class LogFile : noncopyable
{
public:
    LogFile(const std::string &basename,
            off_t rollByteSize = 1024 * 1024 * 1024,
            time_t flusInterval = 3,
            bool threadSafe = true,
            int checkEveryN = 1024);
            
    ~LogFile() = default;

    void append(const char *msg, size_t len);
    void flush();
    void rollFile();

private:
    const std::string basename_;
    const off_t rollByteSize_;
    const time_t flushInterval_;
    const int checkEveryN_;

    time_t start_;              //当前天的零点
    time_t lastRollTime_; 
    time_t lastFlushTime_;
    int count_;                //离上一次rollFile到现在调用append的次数

    std::unique_ptr<MutexLock> lock_;
    std::unique_ptr<AppendFile> file_;

    std::string getFileName(time_t *now);

    static const int kSecondsPerDay = 60 * 60 * 24;

    void append_unlocked(const char *msg, size_t len);
};

#endif 