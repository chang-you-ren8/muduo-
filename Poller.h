#ifndef NET_POLLER_H
#define NET_POLLER_H

#include "noncopyable.h"
#include "Timestamp.h"
#include "EventLoop.h"

#include <map>
#include <vector>

class Channel;

class Poller : noncopyable 
{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop *loop);
    virtual ~Poller();

    virtual Timestamp poll(ChannelList *channelList, int timeoutMs) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;
    
    virtual bool hasChannel(Channel *channel) const; //为什么设置为virutal
    void assertInLoopThread() const;
    static Poller *newDefaultPoller(EventLoop *loop);

protected:
    using ChannelMap = std::map<int, Channel*>;
    ChannelMap fdToChannel_;

private:
    EventLoop *loop_;
};

#endif 