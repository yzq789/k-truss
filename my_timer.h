#include <time.h>

#ifndef __MY_TIMER_H
#define __MY_TIMER_H

class MyTimer{
public:
	void start(char *promt);
	void end();

private:
	clock_t s;
	clock_t e;
	char *promt;
};

#endif