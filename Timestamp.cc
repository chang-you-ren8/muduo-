#include "Timestamp.h"
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif 

const int64_t Timestamp::kMicroSecondsPerSecond = 1000 * 1000;

Timestamp::Timestamp()
	:microSecondsSinceEpoch_(0)
{

}

Timestamp::Timestamp(int64_t microSecondsSinceEpochArg)
	:microSecondsSinceEpoch_(microSecondsSinceEpochArg)
{

}

Timestamp::~Timestamp()
{

}

Timestamp Timestamp::now()
{
	struct timeval tim;
	gettimeofday(&tim, NULL);
	int64_t seconds = tim.tv_sec;
	return Timestamp(seconds * kMicroSecondsPerSecond + tim.tv_usec);
}

std::string Timestamp::toString()const 
{
	char buf[32] = {0};
	int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
	int64_t microSeconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64, seconds, microSeconds);
	return std::string(buf);
}

std::string Timestamp::toDate(bool showMicroSeconds)const
{
	time_t	seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	struct tm date;
	gmtime_r(&seconds, &date);

	char buf[64] = {0};
	if(showMicroSeconds)
	{
		snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d:%06d", 
		date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, 
		date.tm_sec, static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond));
	}
	else 
	{
		snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
		date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min,
		date.tm_sec);
	}
	return std::string(buf);
}

int64_t Timestamp::microSecondsSinceEpoch()const
{
	return  microSecondsSinceEpoch_;
}

int64_t Timestamp::secondsSinceEpoch()const 
{
	return microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
}

double Timestamp::timeDifference(Timestamp high, Timestamp low)
{
	int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / kMicroSecondsPerSecond;		
}

Timestamp Timestamp::addTime(Timestamp init, double seconds)
{
	int64_t microSeconds = init.microSecondsSinceEpoch();
	microSeconds += static_cast<int64_t>(seconds * kMicroSecondsPerSecond);
	return Timestamp(microSeconds);
}

