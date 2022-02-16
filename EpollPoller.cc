#include "EpollPoller.h"
#include "Channel.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <poll.h>

static_assert(POLLIN == EPOLLIN, "POLLIN expect equal to EPOLLIN");
static_assert(POLLOUT == EPOLLOUT, "POLLOUT expect equal to EPOLLOUT");
static_assert(POLLERR == EPOLLERR, "POLLERR expect equal to EPOLLERR");
static_assert(POLLPRI == EPOLLPRI, "POLLPRI expect equal to EPOLLPRI");
static_assert(POLLRDHUP == EPOLLRDHUP, "POLLRDHUP expect equal to EPOLLRDHUP");

const int EpollPoller::kEventListInitalSize;
const int kNew = -1;        //表示这个Channel还未添加到EpollPoller中
const int kAdded = 0;       //表示Channel已经添加
const int kDeleted = 1;     //表示Channel已经被删除

EpollPoller::EpollPoller(EventLoop *loop)
    :   Poller(loop),
        epfd_(::epoll_create1(EPOLL_CLOEXEC)),
        events_(kEventListInitalSize)
{
    assert(epfd_ >= 0);
}

//implicit_cast使用场景？？
Timestamp EpollPoller::poll(ChannelList *channelList, int timeoutMs)
{
    assertInLoopThread();

    int n = ::epoll_wait(epfd_, &*events_.begin(), events_.size(), timeoutMs);

    Timestamp recieveTime = Timestamp::now();
    if(n < 0)
    {
        char msg[64];
        fprintf(stderr, "%s\n", strerror_r(errno, msg, sizeof msg));
    }
    else   
    {
        fillActiveChannel(channelList, n);
        if(n == implicit_cast<int>(events_.size()))
            events_.resize(n << 1);
    }

    return recieveTime; 
}

//channel放在struct epoll_event::data.ptr中
void EpollPoller::fillActiveChannel(ChannelList *channelList, int n)
{
    for(int i = 0; i < n; ++i)
    {
        Channel *ch = static_cast<Channel *>(events_[i].data.ptr);
        ch->setRevents(events_[i].events);
        channelList->push_back(ch);
    }
}

//所有对Channel的修改都只会在其所属的EventLoop线程中被调用
//updateChannel同时处理添加和更新
void EpollPoller::updateChannel(Channel *channel)
{
    assertInLoopThread();

    int state = channel->index();
    int fd = channel->fd();
    auto it = fdToChannel_.find(fd);

    //添加
    if(state == kNew)
    {
        assert(it == fdToChannel_.end());

        channel->setIndex(kAdded);
        fdToChannel_[fd] = channel;
        update(EPOLL_CTL_ADD, channel);
    }//修改
    else if(state == kAdded)
    {
        assert(it != fdToChannel_.end());
        assert(it->second == channel);

        update(EPOLL_CTL_MOD, channel);
    }
    else if(state == kDeleted)
    {
        fprintf(stderr, "%s\n", "EpollPoller::updateChannel Channel has been deleted");
    }
    else  
    {
        fprintf(stderr, "EpollPoller::updateChannel unkown state");
    }
}

void EpollPoller::removeChanel(Channel *channel)
{
    assertInLoopThread();

    int fd = channel->fd();
    auto it = fdToChannel_.find(fd);
    assert(it != fdToChannel_.end() && it->second == channel);
    channel->setIndex(kDeleted);
    fdToChannel_.erase(it);

    update(EPOLL_CTL_DEL, channel);
}

void EpollPoller::update(int op, Channel *channel)
{
    struct epoll_event ev;
    ev.events = channel->events();
    ev.data.ptr = channel; 

    int fd = channel->fd();
    int n = ::epoll_ctl(epfd_, op, fd, &ev);
    assert(n == 0);
}
