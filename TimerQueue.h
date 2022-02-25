#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

//我应该提供什么样的接口？
//基于对象，而不是面向对象
//所以这个TimerQueue应该包含一个Channel对象
//我只应该提供两个接口
//1. addTimer(func, expiration, interval) (用户看不到Timer类)
//2. cancel(TimerId)            //那么TimerId应该如何设计？
//我是否应该做一定的错误处理呢？比如用户让我删除一个不存在的Timer ????

//set<<Timestamp, Timer*>>可以当作小顶堆这个<Timestamp, Timer*>保证唯一
//那么如何删除呢？怎么找到这个定时器呢？？？？
//我为什么一定要和muduo的作者一样呢？
//只要我的代码能实现我的需求不就行了？？？？？？

//每个EventLoop一个TimerQueue。用户是不知道TimerQueue的存在的。
//EventLoop提供四个接口用来处理定时事件.
//1.TimerId runAt(Timestamp, TimerCallback)
//2.TimerId runAfter(double, TimerCallback)
//3.TimerId runEvery(double, TimerCallback)
//4.void cancel(TimerId)

//TimerQueue提供两个线程安全的外部接口
//1.TimerId addTimer(TimerCallback cb, Timestamp when, double interval)
//2.void cancel(TimerId timerId)

#include "TimerId.h"
#include "Timer.h"

#include <set>
#include <memory>
#include <vector>
#include <utility>


class EventLoop;
class Channel;

class TimerQueue :  noncopyable
{
public:
    explicit TimerQueue(EventLoop *loop);
    ~TimerQueue();
    
    TimerId  addTimer(TimerCallback cb, Timestamp when, double interval);

    //FixMe
    //这里未作错误处理，如果cancel同一个TimerId会导致coredump
    void cancel(TimerId timerId);

private:
    EventLoop *loop_;
    int timerfd_;
	std::unique_ptr<Channel> channel_;

    //能不能把这个Timer *换成unique_ptr呢？
    //shared_ptr可以吗？
    using Entry = std::pair<Timestamp, Timer *>;
    std::set<Entry> timerList_;
    std::set<Entry> cancelingTimers_;
    bool callingExpiredTimers_;

    void handleRead();

    std::vector<Entry> getExpiredTimers(Timestamp now);
    void reset(const std::vector<Entry> &expiredTimers, Timestamp now);
    void cancelInLoop(TimerId timerId);

    TimerId addTimerInLoop(Timer *timer);
    bool insert(Timer *timer);
};

#endif 
