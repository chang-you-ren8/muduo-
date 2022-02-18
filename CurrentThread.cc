#include "CurrentThread.h"

#include <sys/types.h>

__thread pid_t t_tid = 0;

pid_t CurrentThread::tid()
{

}