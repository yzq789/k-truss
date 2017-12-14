#include <iostream>
#include "my_timer.h"

using namespace std;

void MyTimer::start(char *promt){
	cout << promt << " starting..." << endl;
	this->promt = promt;
	s = clock();
}
void MyTimer::end(){
	e = clock();
	int seconds = (int)((float)(e - s) / CLOCKS_PER_SEC);
	cout << promt << " end. running time: " << seconds << " s" << endl;;
}