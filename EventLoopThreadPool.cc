#include "EventLoopThreadPool.h"
#include "EventLoop.h"

#include <assert.h>

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop)
    :   started_(false),
        threadNum_(0),
        nextIdx_(0),
        baseLoop_(baseLoop)
{

}


//why ? do something ?
EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::setThreadNum(int threadNum)
{
    baseLoop_->assertInLoopThread();

    assert(threadNum >= 0);
    threadNum_ = threadNum;
}

//only once
void EventLoopThreadPool::start()
{
    baseLoop_->assertInLoopThread();

    assert(!started_);
    started_ = true;
    for(int i = 0; i < threadNum_; ++i)
    {
        threads_.emplace_back(new EventLoopThread);
    }
    for(int i = 0; i < threadNum_; ++i)
    {
        loops_.push_back(threads_[i]->startLoop());
    }
}

EventLoop *EventLoopThreadPool::getNextLoop() 
{
    baseLoop_->assertInLoopThread();

    EventLoop *loop = baseLoop_;
    if(loops_.empty())
        return loop;
    loop = loops_[nextIdx_++];
    if(nextIdx_ == static_cast<int>(loops_.size()))
        nextIdx_ = 0;
    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoop() const
{
    baseLoop_->assertInLoopThread();

    std::vector<EventLoop*> loops;
    if(loops_.empty())
        return {baseLoop_};
    else    
        loops = loops_;
    return loops;
}

