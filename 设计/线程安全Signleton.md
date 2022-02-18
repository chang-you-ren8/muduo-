- 使用pthread_once来做到线程安全。
- pthread_once(&once_, run)确保函数run只会被执行一次。（线程安全）
- 通过在run函数中来构造对象。


**以下Signleton只能进行默认构造**
```c++
#ifndef BASE_SINGLETON_H
#define BASE_SINGLETON_H

#include "noncopyable.h"

#include <pthread.h>
#include <assert.h>

//对外接口只有instance方法
template<typename T>
class Singleton : noncopyable
{
public:
    static T& instance()
    {
        pthread_once(&once_, init);
        assert(value_ != NULL);
        return *value_;
    }

    //无法创建Signleton对象
    Singleton() = delete;
    ~Singleton() = delete;

private:

    static void init()
    {
        value_ = new T;
    }

    static T *value_;
    static pthread_once_t once_;
};

//必须在头文件中定义
template<typename T>
T *Singleton<T>::value_ = NULL;

template<typename T>
pthread_once_t Singleton<T>::once_ = PTHREAD_ONCE_INIT;

#endif 
```