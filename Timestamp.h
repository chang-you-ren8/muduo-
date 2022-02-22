#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

#include "copyable.h"
#include <stdint.h>

//这个类住要时干什么的呢？
//UTC时间，微妙精度
class Timestamp: copyable
{
public:
    Timestamp()
        :   microSecondsSinceEpoch_(0)
    { }

    explicit Timestamp(int64_t microSecondsSinceEpoch)
        :   microSecondsSinceEpoch_(microSecondsSinceEpoch)
    { }

    int64_t microSecondsSinceEpoch() const 
    { return microSecondsSinceEpoch_; }

    bool valid() const 
    { return microSecondsSinceEpoch_ > 0; }

    static Timestamp now();
    static Timestamp invalid();
    static double timeDifference(Timestamp high, Timestamp low);

private:
    int64_t microSecondsSinceEpoch_;

    static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
};

#endif