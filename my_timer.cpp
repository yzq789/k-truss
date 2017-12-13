#include <stdio.h>
#include "my_timer.h"

void MyTimer::start(char *promt){
	printf("%s starting...\n", promt);
	this->promt = promt;
	s = clock();
}
void MyTimer::end(){
	e = clock();
	printf("%s end. running time: %.2f s\n", promt, ((float)(e-s)) / CLOCKS_PER_SEC);
}