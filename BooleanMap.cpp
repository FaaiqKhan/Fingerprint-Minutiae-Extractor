#include "BooleanMap.hpp"
BooleanMap::BooleanMap(int localWidth, int localHeight) {
	width = localWidth;
	height = localHeight;
	array = std::vector<bool>(width * height, false);
}
BooleanMap::BooleanMap(Cell localSize) {
	BooleanMap(localSize.x, localSize.y);
}
BooleanMap::BooleanMap(BooleanMap* localOther) {
	BooleanMap(localOther->size());
	for (int i = 0; i < array.size(); ++i) {
		array[i] = localOther->array[i];
	}
}
Cell BooleanMap::size() {
	return Cell(width, height);
}
bool BooleanMap::get(int x, int y) {
	return array[offset(x, y)];
}
bool BooleanMap::get(Cell at) {
	return get(at.x, at.y);
}
bool BooleanMap::get(int x, int y, bool fallback) {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return fallback;
	}
	return array[offset(x, y)];
}
bool BooleanMap::get(Cell at, bool fallback) {
	return get(at.x, at.y, fallback);
}
void BooleanMap::set(int x, int y, bool value) {
	array[offset(x, y)] = value;
}
void BooleanMap::set(Cell at, bool value) {
	set(at.x, at.y, value);
}
void BooleanMap::invert() {
	for (int i = 0; i < array.size(); ++i) {
		array[i] = !array[i];
	}
}
void BooleanMap::merge(BooleanMap other) {
	if (other.width != width || other.height != height) {
		std::cout<<"IllegalArgumentException" << std::endl;
	} else {
		for (int i = 0; i < array.size(); ++i) {
			bool temp = (bool)other.array[i];
			bool temp1 = array[i];
			bool resultTemp = temp1 |= temp;
			array[i] = resultTemp;
		}
	}
}
int BooleanMap::offset(int x, int y) {
	return y * width + x;
}
