#include "Histogram.hpp"
Histogram::Histogram(int localWidth, int localHeight, int localDepth) {
	width = localWidth;
	height = localHeight;
	depth = localDepth;
	array = std::vector<int>(localWidth * localHeight * localDepth, 0);
}
int Histogram::constrain(int z) {
	return fmax(0, fmin(depth - 1, z));
}
int Histogram::get(int x, int y, int z) {
	return array[offset(x, y, z)];
}
int Histogram::get(Cell at, int z) {
	return get(at.x, at.y, z);
}
int Histogram::sum(int x, int y) {
	int sum = 0;
	for (int i = 0; i < depth; ++i) {
		sum = sum + get(x, y, i);
	}
	return sum;
}
int Histogram::sum(Cell at) {
	return sum(at.x, at.y);
}
void Histogram::set(int x, int y, int z, int value) {
	array[offset(x, y, z)] = value;
}
void Histogram::set(Cell at, int z, int value) {
	set(at.x, at.y, z, value);
}
void Histogram::add(int x, int y, int z, int value) {
	array[offset(x, y, z)] += value;
}
void Histogram::add(Cell at, int z, int value) {
	add(at.x, at.y, z, value);
}
void Histogram::increment(int x, int y, int z) {
	add(x, y, z, 1);
}
void Histogram::increment(Cell at, int z) {
	increment(at.x, at.y, z);
}
int Histogram::offset(int x, int y, int z) {
	return (y * width + x) * depth + z;
}
