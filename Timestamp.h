#ifndef _BASE_TIMESTAMP_H_
#define _BASE_TIMESTAMP_H_

#include "copyable.h"
#include <string>
#include <stdint.h>

class Timestamp : public copyable
{
public:
	
	Timestamp();

	explicit Timestamp(int64_t microSecondsSinceEpochArg);

	~Timestamp();

	static const int64_t kMicroSecondsPerSecond;
	
	static Timestamp now();
	
	static Timestamp addTime(Timestamp init, double seconds);

	static double timeDifference(Timestamp high, Timestamp low);
	
	int64_t microSecondsSinceEpoch()const;

	int64_t secondsSinceEpoch()const;

	std::string toString()const;

	std::string toDate(bool showMicroSeconds)const;

private:
	int64_t microSecondsSinceEpoch_;
};

#endif
