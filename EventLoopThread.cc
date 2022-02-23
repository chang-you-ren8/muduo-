//通过EventLoopThread我们也发现TcpConnection::connectEstablished()函数似乎是必须的
//要通过queueInLoop让channel->enableRead()在EventLoop所属线程中得到执行

#include "EventLoopThread.h"
#include "EventLoop.h"

#include <assert.h>

#include <functional>

//不看书注定无知，无聊，无意义

EventLoopThread::EventLoopThread()
    :   thread_(std::bind(&EventLoopThread::threadFunc, this)),
        latch_(1),
        loop_(NULL),
        looping_(false)
{

}

//change later
EventLoopThread::~EventLoopThread() = default;


//thread safe ????
//这个函数只能被调用一次
EventLoop *EventLoopThread::startLoop()
{
    assert(!looping_);
    looping_ = true;
    thread_.start();

    latch_.wait();
    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    loop_ = &loop;
    latch_.countDown();

    loop.loop();
}