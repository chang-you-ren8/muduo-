#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H

#include <type_traits> // for std::is_same

#include <stdio.h>      //for snprintf
#include <unistd.h>     //for syscall
#include <sys/syscall.h> //for syscall
#include <sys/types.h>  //for pid_t

namespace CurrentThread 
{
    //__thread 只能修饰POD类型数据
    //每个线程都有属于自己的
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char *t_threadName = "unkown";
    //静态断言，失败输出第二个参数
    //is_same判断两个类型是否一样
    //编译期确定
    static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");


    //syscall(SYS_gettid)这个是Linux系统调用
    //用来获取线程对应的pid（pthread实现的线程就是轻量级的进程）
    pid_t gettid()
    {
        return static_cast<pid_t>(::syscall(SYS_gettid));
    }

    void cachedTid()
    {
        if(t_cachedTid == 0)
        {
            t_cachedTid = gettid();
            t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        }
    }

    inline int tid()
    {
        //gcc内置的分支预测优化
        if(__builtin_expect(t_cachedTid == 0, 0))
            cachedTid();
        return t_cachedTid;
    }

    inline const char *tidString()
    {
        return t_tidString;
    }

    inline int tidStringLength()
    {
        return t_tidStringLength;
    }

    inline const char *name()
    {
        return t_threadName;
    }
}

#endif 