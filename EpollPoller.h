#ifndef NET_EPOLLPOLLER_H
#define NET_EPOLLPOLLER_H

#include "Poller.h"

#include <sys/epoll.h>

#include <vector>

class EventLoop;

class EpollPoller : public Poller
{
public:
    explicit EpollPoller(EventLoop *loop);
    ~EpollPoller() override;

    Timestamp poll(ChannelList *activeChannelList, int timeoutMs) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;

private:
    using EventList = std::vector<struct epoll_event>;
    int epfd_;
    EventList events_; 

    void fillActiveChannels(ChannelList *activeChannelList, int n);
    void update(Channel *channel, int op);

    static const int kEventListInitSize = 8;
    static const int kNew = -1;
    static const int kAdded = 1;
    static const int kDeleted = 2;
};

#endif 