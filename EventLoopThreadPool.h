#ifndef NET_EVENTLOOPTHREADPOOL_H
#define NET_EVENTLOOPTHREADPOOL_H

#include "EventLoopThread.h"

#include <vector>
#include <memory>

//没有需求就没有设计思路。
class EventLoopThreadPool : noncopyable
{
public:
    explicit EventLoopThreadPool(EventLoop *baseLoop);
    ~EventLoopThreadPool();

    void setThreadNum(int threadNum);
    void start();
    EventLoop *getNextLoop();
    std::vector<EventLoop *> getAllLoop() const;

private:
    bool started_;
    int threadNum_;
    int nextIdx_;       //for round-robin
    EventLoop *baseLoop_;
    std::vector<EventLoop *>loops_;     //EventLoop* 是栈上对象
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
};

#endif 