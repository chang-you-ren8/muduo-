//timerfd
/**
 * <sys/timerfd.h>
 * int timerfd_create(int clockid, int flags)
 * int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value)
 * int timerfd_gettime(int fd, struct itimerspec *curr_value)
 **/
//clockid : CLOCK_REALTIME, CLOCK_MONOTONIC (linux >= 2.6.27)
//flags:TFD_NONBLOCK, TFD_CLOEXEC
/**
 * struct timespec{
 *     time_t tv_sec;
 *     long tv_nsec;
 * }
 * 
 * struct itimerpsec{
 *      struct timespec it_interval;
 *      struct timespec it_value;
 * }
 **/

#include "TimerQueue.h"
#include "Channel.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <algorithm>

int createTimerfdOrDie()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    assert(timerfd >= 0);
    return timerfd;
}


void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t val;
    ssize_t n = ::read(timerfd, &val, sizeof val);
    assert(n == sizeof(val));
    (void)n;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microSeconds = Timestamp::timeDifference(when, Timestamp::now());

    if(microSeconds < 100)
    {
        microSeconds = 100;
    }

    struct timespec spec;
    spec.tv_sec = microSeconds / Timestamp::kMicroSecondsPerSecond; 
    spec.tv_nsec = microSeconds % Timestamp::kMicroSecondsPerSecond * 1000;
    return spec;
}

void resetTimerfd(int timerfd, Timestamp nextExpiredTime)
{
    //一次性的啊。。。
    struct timespec it = howMuchTimeFromNow(nextExpiredTime);
    struct itimerspec oldValue;
    struct itimerspec newValue;
    ::bzero(&oldValue, sizeof oldValue);
    ::bzero(&newValue, sizeof newValue);
    newValue.it_value = it; 

    //timerfd_settime(timerfd, flag, newValue, oldValue);
    //flag 取 0 表示使用的是相对的时间
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    assert(ret == 0);
    (void)ret;
}

TimerQueue::TimerQueue(EventLoop *loop)
    :   loop_(loop),
        timerfd_(createTimerfdOrDie()),
		channel_(new Channel(loop, timerfd_)),
        callingExpiredTimers_(false)
{
	//我有个疑问，这个Loop此时还没有构造完成吧？？？？？？？
	//我测试的时候发现loop->updateChannel发生segmentFault.......
    assert(loop_ != NULL);
    channel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
    channel_->enableRead();
}


TimerQueue::~TimerQueue()
{
    for(const auto &it : timerList_)
    {
        //如何避免使用裸指针呢？
        delete it.second;
    }

    ::close(timerfd_);
}

//实现到这里我突然思考，TcpConnection为什么使用shared_ptr,如果对于TcpConnection我只使用裸指针会怎么样？
TimerId TimerQueue::addTimer(TimerCallback cb, Timestamp when, double interval)
{
    Timer *timer = new Timer(cb, when, interval);
    loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
    return TimerId(timer);
}


void TimerQueue::cancel(TimerId timerId)
{
    loop_->runInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

//当有超时事件发生时，EventLoop负责回调handleRead
void TimerQueue::handleRead()
{
    loop_->assertInLoopThread();

    //FixMe
    Timestamp now(Timestamp::now());
    readTimerfd(timerfd_, now);

    std::vector<Entry> expired = getExpiredTimers(now);

    callingExpiredTimers_ = true;
    cancelingTimers_.clear();
    for(const auto &entry: expired)
    {
        entry.second->run();
    }
    callingExpiredTimers_ = false;

    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpiredTimers(Timestamp now)
{   
    loop_->assertInLoopThread();

    //将超时时间小于等于now的Timer取出
    //<now, 地址最大值>
    //UINTPTR_MAX 就是指针的最大值
    std::vector<Entry> expired;

    Entry entry(now, (Timer *)UINTPTR_MAX);
    auto end = timerList_.lower_bound(entry);
    std::copy(timerList_.begin(), end, back_inserter(expired));
    timerList_.erase(timerList_.begin(), end);

    return expired;
} 

TimerId TimerQueue::addTimerInLoop(Timer *timer)
{
    loop_->assertInLoopThread();

    bool expiredTimeChanged = insert(timer);
    if(expiredTimeChanged)
    {
        resetTimerfd(timerfd_, timer->expiration());
    }
	
	return TimerId(timer);
}

bool TimerQueue::insert(Timer *timer)
{
    //最早超时时间是否发生变化
    bool changed = false;
    if(timerList_.empty() || timer->expiration() < timerList_.begin()->first)
        changed = true;
    timerList_.insert(Entry(timer->expiration(), timer));
    return changed;
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
    loop_->assertInLoopThread();

    Timer *timer = timerId.timer();
    Entry entry(timer->expiration(), timer);
    auto it = timerList_.find(entry);
    if(it != timerList_.end())
    {
        delete timer;
        timerList_.erase(it);
    }
    else   
    {
        assert(callingExpiredTimers_);
        cancelingTimers_.insert(entry);
    }
}

void TimerQueue::reset(const std::vector<Entry> &expired, Timestamp now)
{
    loop_->assertInLoopThread();

    Timestamp nextExpiredTime;

    for(auto &entry: expired)
    {
        Timer *timer = entry.second;
        if(timer->repeat() && cancelingTimers_.find(entry) == cancelingTimers_.end())
        {
            timer->restart(now);
            timerList_.insert(Entry(timer->expiration(), timer));
        }
        else
        {
            delete timer;
        }
    }

    if(!timerList_.empty())
    {
        nextExpiredTime = timerList_.begin()->first;
    }

    if(nextExpiredTime.valid())
    {
        resetTimerfd(timerfd_, nextExpiredTime);
    }
}

