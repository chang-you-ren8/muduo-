#ifndef NET_POLLER_H
#define NET_POLLER_H

#include "noncopyable.h"
#include "Timestamp.h"

#include <map>
#include <vector>

class Channel;
class EventLoop;

class Poller : noncopyable
{
public:
    using ChannelList = std::vector<Channel *>;

    explicit Poller(EventLoop *loop);
    virtual ~Poller();

    virtual Timestamp poll(ChannelList *activeChannelList, int timeoutMs) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;
    virtual bool hasChannel(Channel *channel) const;

    void assertInLoopThread() const;

    static Poller *newDefaultPoller(EventLoop *loop);

protected:
    using ChannelMap = std::map<int,Channel*>;
    ChannelMap fdToChannel_;

private:
    EventLoop *loop_;
};

#endif 