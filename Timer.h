#ifndef NET_TIMER_H
#define NET_TIMER_H

//如何设计定时器？
//定时器可以拷贝
//这个Timer应该什么也不做 ???

#include "copyable.h"
#include "AtomicInteger.h"
#include "Timestamp.h"
#include "Callbacks.h"

#include <functional>

class Timer : copyable
{
public:
    Timer(TimerCallback cb, Timestamp when, double interval)
        :   cb_(std::move(cb)),
            expiration_(when),
            repeat_(interval > 0.0),
            interval_(interval),
			sequence_(s_numCreate_.getAndIncrement())
    { }


    void run()
    { cb_(); }

    Timestamp expiration() const 
    { return expiration_; }

    bool repeat() const   
    { return repeat_; }

    double interval() const   
    { return interval_; }

    int64_t sequence() const
    { return sequence_; }

    void restart(Timestamp now);

private:
    const TimerCallback cb_;
    Timestamp expiration_;
    const bool repeat_;
    const double interval_;
    const int64_t sequence_;

    static AtomicInt64_t  s_numCreate_;
};

#endif 
