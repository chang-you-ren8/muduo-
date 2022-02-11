#include "EpollPoller.h"

const int EpollPoller::kNew = -1;
const int EpollPoller::kAdded = 1;
const int EpollPoller::kDel = 2;
const int EpollPoller::kInitEventListSize= 16;

EpollPoller::EpollPoller(EventLoop *loop)
	:Poller(loop)
	,epfd_(::epoll_create1(EPOLL_CLOEXEC))
	,eventList_(kInitEventListSize)
{
	assert(epfd_ >= 0);
}

EpollPoller::~EpollPoller()
{
	::close(epfd_);
}

Timestamp EpollPoller::poll(ChannelList *activeChannelList, int timeoutMs)
{
	int numEvent = ::epoll_wait(epfd_, &*eventList_.begin(), static_cast<int>(eventList_.size()), timeoutMs);

	Timestamp now = Timestamp::now();

	if(numEvent < 0)
	{
		if(errno != EINTR)
			assert(0);
	}

	if(numEvent > 0)
		fillActiveChannel(activeChannelList, numEvent);
	
	return now;
}

void EpollPoller::fillActiveChannel(ChannelList *activeChannelList, int numEvent)
{
	for(int i = 0; i < numEvent; ++i)
	{
		Channel * channel = static_cast<Channel *>(eventList_[i].data.ptr);

		channel->setRevents(eventList_[i].events);
		activeChannelList->push_back(channel);
	}
	if(numEvent == static_cast<int>(eventList_.size()))
		eventList_.resize(numEvent * 2);
}

void EpollPoller::updateChannel(Channel *channel)
{
	int index = channel->index();
	int fd = channel->fd();

	if(index == kNew)
	{
		assert(fdToChannel_.find(fd) == fdToChannel_.end());

		channel->setIndex(kAdded);
		fdToChannel_[fd] = channel;

		update(channel, EPOLL_CTL_ADD);
	}
	else
	{
		auto ch = fdToChannel_.find(fd);
		assert(ch != fdToChannel_.end() && ch->second == channel);

		update(channel, EPOLL_CTL_MOD);
	}
}


void EpollPoller::removeChannel(Channel *channel)
{
	int fd = channel->fd();	
	auto ch = fdToChannel_.find(fd);
	assert(ch != fdToChannel_.end() && ch->second == channel);

	channel->setIndex(kDel);
	fdToChannel_.erase(fd);
	update(channel, EPOLL_CTL_DEL);
}

bool EpollPoller::hasChannel(Channel *channel)
{
	int fd = channel->fd();
	auto ch = fdToChannel_.find(fd);
	if(ch != fdToChannel_.end() && ch->second == channel)
		return true;
	return false;
}

void EpollPoller::update(Channel *channel, int mode)
{
	int fd = channel->fd();
	int events = channel->events();

	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.ptr = channel;
	ev.events = events;

	assert(::epoll_ctl(epfd_, mode, fd, &ev) == 0);
}





