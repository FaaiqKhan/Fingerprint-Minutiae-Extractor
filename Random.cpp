#include "Random.hpp"
myRandom::myRandom() {
	setSeed((long long) time(0));
}
void myRandom::setSeed(long long localSeed) {
	temp = ((localSeed ^ 0x5DEECE66DLL) & ((1LL << 48) - 1));
	seed = &temp;
	*seed = temp;
}
int myRandom::next(int bits) {
	temp2 = ((long long)((*this->seed) *  0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1));
	this->seed = &temp2;
	*this->seed = temp2;
	return (int) (((unsigned long long) (*this->seed)) >> (48 - bits));
}
double myRandom::nextDouble() {
	double val = ((double)(((long long) next(26)) << 27) + next(27)) / (double) (1LL << 53);
	return val;
}
