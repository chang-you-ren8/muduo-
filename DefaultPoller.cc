#include "Poller.h"
#include "EpollPoller.h"
#include "EventLoop.h"


Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    return new EpollPoller(loop);
}