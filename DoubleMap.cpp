#include "DoubleMap.hpp"
DoubleMap::DoubleMap(int localWidth, int localHeight) {
	width = localWidth;
	height = localHeight;
	array = std::vector<double>(width*height, 0);
}
DoubleMap::DoubleMap(Cell size) {
	DoubleMap(size.x, size.y);
}
Cell DoubleMap::size() {
	return Cell(width, height);
}
double DoubleMap::get(int x, int y) {
	return array[offset(x, y)];
}
double DoubleMap::get(Cell at) {
	return get(at.x, at.y);
}
void DoubleMap::set(int x, int y, double value) {
	array[offset(x, y)] = value;
}
void DoubleMap::set(Cell at, double value) {
	set(at.x, at.y, value);
}
void DoubleMap::add(int x, int y, double value) {
	array[offset(x, y)] += value;
}
void DoubleMap::add(Cell at, double value) {
	add(at.x, at.y, value);
}
void DoubleMap::multiply(int x, int y, double value) {
	array[offset(x, y)] *= value;
}
void DoubleMap::multiply(Cell at, double value) {
	multiply(at.x, at.y, value);
}
int DoubleMap::offset(int x, int y) {
	return y * width + x;
}
