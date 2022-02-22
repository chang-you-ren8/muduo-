#include "CurrentThread.h"

namespace CurrentThread
{
    __thread pid_t t_cachedTid = 0;
}