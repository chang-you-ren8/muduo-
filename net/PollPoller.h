#ifndef _NET_POLLPOLLER_H_
#define _NET_POLLPOLLER_H_

#include "Poller.h"
#include "Channel.h"

#include <poll.h>
#include <assert.h>

#include <map>


class PollPoller:public Poller
{
public:
	PollPoller(EventLoop *loop);
	~PollPoller() override;

	Timestamp poll(ChannelList *activeChannelList, int timeoutMs) override;
	void updateChannel(Channel *channel) override;
	void removeChannel(Channel *channel) override;
	bool hasChannel(Channel *channel) override;

private:
	using ChannelMap = std::map<int, Channel*>;
	using PollFdList = std::vector<struct pollfd>;
	
	PollFdList pfdList_;
	ChannelMap fdToChannel_;
	
	void fillActiveChannelList(ChannelList *activeChannelList, int eventNum);
};

#endif
