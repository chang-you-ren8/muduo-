#ifndef NET_EPOLLPOLLER_H
#define NET_EPOLLPOLLER_H

#include "Poller.h"

#include <sys/epoll.h>

#include <vector>
//就是对epoll_wait, epoll_ctl的封装
class EventLoop;

class EpollPoller : public Poller 
{
public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller();

    Timestamp poll(ChannelList *channelList, int timeoutMs) override;
    void updateChannel(Channel *channel) override;
    void removeChanel(Channel *channel) override;

private:
    using EventList = std::vector<struct epoll_event>;
    static const int kEventListInitalSize = 16;

    int epfd_;
    EventList events_; 


    void fillActiveChannel(ChannelList *channelList, int n);
    void update(int op, Channel *channel);
};

#endif 