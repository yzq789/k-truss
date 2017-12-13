#include <time.h>

class MyTimer{
public:
	void start(char *promt);
	void end();

private:
	clock_t s;
	clock_t e;
	char *promt;
};