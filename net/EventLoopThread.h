#ifndef NET_EVETLOOPTHREAD_H
#define NET_EVETLOOPTHREAD_H  

//说白了就是让这个类包含一个Thread对象。
//基于对象的设计方法。

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include "noncopyable.h"

#include <string>
#include <functional>

class EventLoop;

class EventLoopThread : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;

    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    const std::string &name = string());
};

#endif 