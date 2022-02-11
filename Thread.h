#ifndef BASE_THREAD_H   
#define BASE_THREAD_H  

#include "noncopyable.h"
#include "CountDownLatch.h"

#include <pthread.h>
#include <sys/types.h>

#include <functional>

class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(const ThreadFunc &cb);
    ~Thread();

    void start();
    bool started();
    void join();

    pid_t tid() const; //线程对应的进程id(唯一的)(pthread实现的线程实际上是个轻量级进程)

private:
    ThreadFunc func_;
    pthread_t pthreadId_;
    pid_t tid_;
    CountDownLatch latch_;
    bool running_;
    bool joined_;
};

#endif 