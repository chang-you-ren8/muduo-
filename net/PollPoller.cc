#include "PollPoller.h"


PollPoller::PollPoller(EventLoop *loop)
	:Poller(loop)
{

}

PollPoller::~PollPoller() = default;

Timestamp PollPoller::poll(ChannelList *activeChannelList, int timeoutMs)
{
	int eventNum = ::poll(&*pfdList_.begin(), pfdList_.size(), timeoutMs);
	
	Timestamp now = Timestamp::now();

	fillActiveChannelList(activeChannelList, eventNum);

	return now;
}

void PollPoller::fillActiveChannelList(ChannelList *activeChannelList, int eventNum)
{
	for(auto it = pfdList_.cbegin(); it != pfdList_.cend() && eventNum > 0; ++it)
	{
		if(it->revents > 0)
		{
			eventNum -= 1;

			int fd = it->fd;
			auto ch = fdToChannel_.find(fd);
			assert(ch != fdToChannel_.end());
			Channel *channel = ch->second;
			channel->setRevents(it->revents);
			activeChannelList->push_back(channel);
		}
	}
}

//struct pollfd 的 revents 是否需要初始化？
void PollPoller::updateChannel(Channel *channel)
{
	int index = channel->index();
	int fd = channel->fd();
	//还未添加
	if(index < 0)
	{
		assert(fdToChannel_.find(fd) == fdToChannel_.end());
		
		fdToChannel_[fd] = channel;
		struct pollfd pfd;
		pfd.fd = fd;
		pfd.events = channel->events();
		pfd.revents = 0;
		pfdList_.push_back(pfd);
		channel->setIndex(pfdList_.size() - 1);
	}
	else
	{
		auto ch = fdToChannel_.find(fd);
		assert(ch != fdToChannel_.end() && ch->second == channel);
		assert (index >= 0 && index < static_cast<int>(pfdList_.size()));

		pfdList_[index].events = channel->events();
	}
}


void PollPoller::removeChannel(Channel *channel)
{
	int fd = channel->fd();
	int index = channel->index();
	assert(fdToChannel_.find(fd) != fdToChannel_.end());
	assert(index >= 0 && index < static_cast<int>(pfdList_.size()));

	fdToChannel_.erase(fd);
	if(index != static_cast<int>(pfdList_.size()) - 1)
	{
		//和末尾的pfd交换
		int p = pfdList_.size() - 1;
		Channel *backChannel = fdToChannel_[pfdList_[p].fd];
		backChannel->setIndex(index);
		std::swap(pfdList_[index], pfdList_[p]);
	}
	channel->setIndex(-1);
	pfdList_.pop_back();
}

bool PollPoller::hasChannel(Channel *channel)
{
	int fd = channel->fd();
	auto ch = fdToChannel_.find(fd);
	if(ch != fdToChannel_.end() && ch->second == channel)
		return true;
	return false;
}



