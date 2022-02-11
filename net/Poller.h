#ifndef _NET_POLLER_H_
#define _NET_POLLER_H_

#include "base/noncopyable.h"
#include "base/Timestamp.h"

#include <vector>

class Channel;
class EventLoop;

class Poller: public noncopyable 
{

public:
	using ChannelList = std::vector<Channel*>;

	Poller(EventLoop *loop);

	virtual ~Poller();

	virtual Timestamp poll(ChannelList *activeChannelList, int timeoutMs = kPollTimeoutMs) = 0;
	virtual void updateChannel(Channel *channel) = 0;
	virtual	void removeChannel(Channel *channel) = 0;
	virtual bool hasChannel(Channel *channel) = 0;

private:
	EventLoop *loop_;
	static const int kPollTimeoutMs = 1000 * 1000;
};

#endif 
