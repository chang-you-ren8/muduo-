#ifndef _NET_EPOLLPOLLER_H_
#define _NET_EPOLLPOLLER_H_

#include "Poller.h"
#include "Channel.h"

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/epoll.h>

#include <map>


class EventLoop;

class EpollPoller: public Poller
{
public:
	EpollPoller(EventLoop *loop);
	~EpollPoller() override; // 为什么要加override

	Timestamp poll(ChannelList *activeChannelList, int timeoutMs) override;
	
	void updateChannel(Channel *channel) override;
	void removeChannel(Channel *channel) override;
	bool hasChannel(Channel *channel) override;

private:	
	using ChannelMap = std::map<int, Channel*>;
	using EpollEventList = std::vector<struct epoll_event>;

	//channel->index() == kNew表示还未添加
	//kNew = -1
	static const int kNew;
	static const int kAdded;
	static const int kDel;
	static const int kInitEventListSize;

	int epfd_;
	ChannelMap fdToChannel_;
	EpollEventList eventList_;


	void fillActiveChannel(ChannelList *activeChannelList, int numEvent);
	void update(Channel *channel, int mode);
};

#endif
