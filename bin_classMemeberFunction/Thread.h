#include <pthread.h>

#include <assert.h>

#include <functional>

class Thread 
{
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(const ThreadFunc &func)
        : threadFunc_(func),
          running_(false),
          joined_(false)
    { }

    ~Thread()
    {
        if(running_ && !joined_)
            assert(pthread_detach(tid_) == 0);
    }

    void start()
    {
        assert(!running_);
        running_ = true;
        assert(pthread_create(&tid_, NULL, runInThread, this) == 0);
    }

    void join()
    {
        if(running_ && !joined_)
        {
            running_ = false;
            joined_ = true;
            assert(pthread_join(tid_, NULL) == 0);
        }
    }

    void run()
    {
        threadFunc_();
    }

private:
    ThreadFunc threadFunc_;
    bool running_;
    bool joined_;
    pthread_t tid_;

    static void *runInThread(void *);
};

void *Thread::runInThread(void *arg)
{
    Thread *thread = static_cast<Thread *>(arg);
    thread->run();
    return NULL;
}