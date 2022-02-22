#include "Poller.h"
#include "EpollPoller.h"


class EventLoop;

Poller* Poller::newDefaultPoller(EventLoop *loop)
{
    return new EpollPoller(loop);
}