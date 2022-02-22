#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H

#include <sys/types.h> //for pid_t
#include <sys/syscall.h> //for SYS_gettid 
#include <unistd.h>         //for syscall

namespace CurrentThread
{
    extern __thread pid_t t_cachedTid;

    inline void cachedTid()
    {
        t_cachedTid = ::syscall(SYS_gettid);
    }

    inline pid_t tid()
    {
        if(t_cachedTid == 0)
            cachedTid();
        return t_cachedTid;
    }


} //CurrentThread

#endif