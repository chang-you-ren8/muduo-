#ifndef _BASE_SINGLEINSTANCE_H_
#define _BASE_SINGLEINSTANCE_H_

#include "noncopyable.h"

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

template<typename T>
class SingleInstance: noncopyable
{
public:
    SingleInstance() = delete;
    ~SingleInstance() = delete;

    static T* instance()
    {
        pthread_once(&once_, init);
        assert(value_ != NULL);
        return value_;
    }

private:
    //init 和 destroy 的实现还有漏洞
    static void init()
    {
        value_ = new T();
        atexit(destroy); 
    }

    static void destroy()
    {
        delete value_;
        value_ = NULL;
    }

    static T *value_;
    static pthread_once_t once_;
};

template<typename T>
T *SingleInstance<T>::value_ = NULL;

template<typename T>
pthread_once_t SingleInstance<T>::once_ = PTHREAD_ONCE_INIT;

#endif 