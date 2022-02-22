#include "EpollPoller.h"
#include "EventLoop.h"
#include "Channel.h"

#include <string.h>

static_assert(EPOLLIN == POLLIN, "EPOLLIN expect to equal POLLIN");
static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT expect to equal POLLOUT");
static_assert(EPOLLERR == POLLERR, "EPOLLERR expect to equal POLLERR");
static_assert(EPOLLRDHUP == POLLRDHUP, "EPOLLRDHUP expect to equal POLLRDHUP");

const int EpollPoller::kEventListInitSize;
const int EpollPoller::kNew;
const int EpollPoller::kAdded;
const int EpollPoller::kDeleted;

EpollPoller::EpollPoller(EventLoop *loop)
    :   Poller::Poller(loop),
        epfd_(::epoll_create1(EPOLL_CLOEXEC)),
        events_(kEventListInitSize)
{

}


EpollPoller::~EpollPoller()
{
    ::close(epfd_);
}

Timestamp EpollPoller::poll(ChannelList *activeChannelList, int timeoutMs)
{
    int n = ::epoll_wait(epfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    Timestamp now = Timestamp::now();
    assert(n >= 0);
    if(n > 0)
        fillActiveChannels(activeChannelList, n);

    if(static_cast<int>(events_.size()) == n)
        events_.resize(n << 1);

    return now;
}

void EpollPoller::fillActiveChannels(ChannelList *activeChannelList, int n)
{
    for(int i = 0; i < n; ++i)
    {
        Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
        channel->setRevents(events_[i].events);
        activeChannelList->push_back(channel);
    }
}

void EpollPoller::updateChannel(Channel *channel)
{
    assertInLoopThread();

    int idx = channel->index();
    int fd = channel->fd();
    if(idx == kNew)
    {
        channel->setIndex(kAdded);
        fdToChannel_[fd] = channel;
        update(channel, EPOLL_CTL_ADD);
    }
    else if(idx == kAdded)
    {
        assert(hasChannel(channel));
        update(channel, EPOLL_CTL_MOD);
    }
    else  
    {
        assert(0);
    }
}

void EpollPoller::removeChannel(Channel *channel)
{
    assertInLoopThread();

    assert(channel->index() == kAdded);
    assert(hasChannel(channel));
    int fd = channel->fd();
    auto n = fdToChannel_.erase(fd);
    assert(n == 1);
    channel->setIndex(kDeleted);
    update(channel, EPOLL_CTL_DEL);
}

void EpollPoller::update(Channel *channel, int op)
{
    struct epoll_event ev;
    ::bzero(&ev, sizeof(ev));
    ev.events = channel->events();
    ev.data.ptr = channel;          //key
    int ret = ::epoll_ctl(epfd_, op, channel->fd(), &ev);
    assert(ret == 0);
}