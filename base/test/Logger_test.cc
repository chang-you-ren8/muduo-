#include "Logger.h"

void output(const char *logline, int len)
{
	fprintf(stdout, "%s", logline);
}

void flush()
{
	fflush(stdout);
}

int main(void)
{
	Logger::setOutputFunc(output);
	Logger::setFlushFunc(flush);
	Logger::setLogLevel(Logger::INFO);

	for(int i = 0; i < 50; ++i)
		LOG_INFO << "Logger_test.cc";

	return 0;
}