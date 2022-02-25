#include "Timestamp.h"

#include <sys/time.h> //for gettimeofday
#include <time.h>
#include <assert.h>
#include <stdlib.h>  //for NULL ???

const int64_t Timestamp::kMicroSecondsPerSecond;

string Timestamp::toDate() const  
{
    char buf[64];

    int64_t microSeconds = Timestamp::now().microSecondsSinceEpoch();
    time_t seconds = static_cast<time_t>(microSeconds / kMicroSecondsPerSecond);
    struct tm tm_time;
    ::gmtime_r(&seconds, &tm_time);   

    microSeconds = microSeconds % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d.%06d",
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
            static_cast<int>(microSeconds));

    return string(buf);
}

Timestamp Timestamp::now()
{
    struct timeval tm;
    int ret = gettimeofday(&tm, NULL);
    assert(ret == 0);

    int64_t ms = tm.tv_sec * kMicroSecondsPerSecond;
    ms += tm.tv_usec;
    return Timestamp(ms);
}

Timestamp Timestamp::addTime(Timestamp now, double seconds)
{
    int64_t microSeconds = seconds * kMicroSecondsPerSecond;
    microSeconds += now.microSecondsSinceEpoch();
    return Timestamp(microSeconds);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}

double Timestamp::timeDifference(Timestamp high, Timestamp low)
{
    int64_t t = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    double diff = static_cast<double>(t) / kMicroSecondsPerSecond;
    return diff;
}
