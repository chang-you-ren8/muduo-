#ifndef NET_TIMERID_H
#define NET_TIMERID_H

//我不知道需求，所以我不知道如何设计。。。。。
//需求决定一切？？？？
//需求来源与经验？？？？

//如何区分两个不同的定时器，即使他们的Timestamp一样？？？
//Timestamp可能一样吗？？？？？？？？
//我必须考虑这中问题吗？
//<Timestamp, Timer*>就可以唯一确定了
#include "Timer.h"

//暂时先这样实现。
//Timer里面包含了Timestamp，所我可以唯一确定这个Timer
class TimerId : copyable
{
public:
    explicit TimerId(Timer *timer)
        :   timer_(timer)
    { }

    Timer *timer() const   
    { return timer_; }

private:
    Timer *timer_;
};

#endif 