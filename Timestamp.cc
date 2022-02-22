#include "Timestamp.h"

#include <sys/time.h> //for gettimeofday
#include <assert.h>
#include <stdlib.h>  //for NULL ???

const int64_t Timestamp::kMicroSecondsPerSecond;

Timestamp Timestamp::now()
{
    struct timeval tm;
    int ret = gettimeofday(&tm, NULL);
    assert(ret == 0);

    int64_t ms = tm.tv_sec * kMicroSecondsPerSecond;
    ms += tm.tv_usec;
    return Timestamp(ms);
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