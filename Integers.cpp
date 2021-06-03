#include "Integers.hpp"
int Integers::sq(int value) {
	return value * value;
}
int Integers::roundUpDiv(int dividend, int divisor) {
	return (dividend + divisor - 1) / divisor;
}
int Integers::bitCount(int i) {
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    i = (i + (i >> 4)) & 0x0f0f0f0f;
    i = i + (i >> 8);
    i = i + (i >> 16);
    return i & 0x3f;
}
int Integers::compare(int x, int y) {
	return (x < y) ? -1 : ((x == y) ? 0 : 1);
}
