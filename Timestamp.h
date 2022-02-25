#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

#include "copyable.h"
#include <stdint.h>

#include <string>
using std::string;

//这个类住要时干什么的呢？
//UTC时间，微妙精度
//这类要支持比较运算符。
//现在的方法是通过友元函数来实现的。（但是好像可以通过继承某个模板来实现）
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

    string toDate() const;

    static Timestamp now();
    static Timestamp invalid();
    static Timestamp addTime(Timestamp now, double seconds);
    static double timeDifference(Timestamp high, Timestamp low);
    static const int64_t kMicroSecondsPerSecond = 1000 * 1000;

    //内联
    friend bool operator<(Timestamp lhs, Timestamp rhs)
    { return lhs.microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_ ;}

    friend bool operator>=(Timestamp lhs, Timestamp rhs)
    { return !(lhs < rhs); }

    friend bool operator>(Timestamp lhs, Timestamp rhs)
    { return lhs.microSecondsSinceEpoch_ > rhs.microSecondsSinceEpoch_; }

    friend bool operator<=(Timestamp lhs, Timestamp rhs)
    { return !(lhs > rhs); }

private:
    int64_t microSecondsSinceEpoch_;
};

#endif