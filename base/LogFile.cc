#include "MutexLockGuard.h"
#include "LogFile.h"
#include "ProcessInfo.h"

#include <string.h>

const int LogFile::kSecondsPerDay;

LogFile::LogFile(const std::string &basename, off_t rollByteSize, time_t flushInterval, bool threadSafe, int checkEveryN)
    : basename_(basename),
      rollByteSize_(rollByteSize),
      flushInterval_(flushInterval),
      checkEveryN_(checkEveryN),
      start_(0),
      lastRollTime_(0),
      lastFlushTime_(0),
      count_(0),
      lock_(threadSafe ? new MutexLock : NULL)
{
    assert(strchr(basename_.c_str(), '/') == NULL);
    rollFile();
}

void LogFile::append(const char *msg, size_t len)
{
    if(lock_)
    {
        MutexLockGuard guard(*lock_);
        append_unlocked(msg, len);
    }
    else 
        append_unlocked(msg, len);
}

void LogFile::append_unlocked(const char *msg, size_t len)
{
    file_->append(msg, len);
    if(file_->writtenBytes() > rollByteSize_) 
        rollFile();
    else 
    {
        count_ += 1;
        if(count_ >= checkEveryN_) 
        {
            time_t now = ::time(NULL);
            time_t newStart = now / kSecondsPerDay * kSecondsPerDay;
            if(newStart != start_)                          //新的一天
                rollFile();
            else if(now - lastFlushTime_ > flushInterval_) // 超过刷新的时间间隔
                flush();
        }
    } 
}

void LogFile::flush()
{
    count_ = 0;
    lastFlushTime_ = ::time(NULL);

    if(lock_)
    {
        MutexLockGuard guard(*lock_);
        file_->flush();
    }
    else 
        file_->flush();
}

void LogFile::rollFile()
{
    count_ = 0;

    time_t now;
    std::string filename = getFileName(&now); 
    start_ = now / kSecondsPerDay * kSecondsPerDay;
    lastRollTime_ = now;
    lastFlushTime_ = now;

    file_.reset(new AppendFile(filename.c_str()));
}

//basename + 日期（精确到s) + 主机名 + 当前进程的pid + .log
std::string LogFile::getFileName(time_t *now)
{
    std::string filename = basename_;
    filename.reserve(filename.size() + 64);

    char timebuf[32];
    *now = ::time(NULL);
    struct tm tm;
    gmtime_r(now, &tm);
    //size_t strftime(char *s, size_t max, const char *format, const struct tm *tm)
    //自动添加'\0'
    //%Y -- the year as a decimal number including the century
    //%m -- the month as a decimal number (range 01 to 12)
    //%d -- the day of the month as a decimal number (range 01 to 31)
    //%H -- the hour as a decimal number using a 24-hour clock (range 00 to 23)
    //%M -- the minutes as a decimal number (range 00 to 59)
    //%S -- the second as a decimal (range 00 to 60)
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;

    filename += ProcessInfo::hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), ".%d", ProcessInfo::pid());
    filename += pidbuf;

    filename += ".log";
    return filename;
}