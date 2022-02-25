#include "Timer.h"

//说白了这个Timer干了什么呢？
/**
 * 1.超时时要执行的函数，以及提供接口来执行这个函数
 * 2.获得这个定时器的超时时间
 * 3.判断这个定时器是否可重复
 * 4.获得这个定时器的id
 * 5.提供接口用来重启这个定时器
 **/

AtomicInt64_t Timer::s_numCreate_;

void Timer::restart(Timestamp now)
{
    if(repeat())
    {
        expiration_ = Timestamp::addTime(now, interval_);
    }
    else    
    {
        expiration_ = Timestamp::invalid();
    }
}