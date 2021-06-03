#include "PointMap.hpp"
PointMap::PointMap(int localWidht, int localHeight) {
	width = localWidht;
	height = localHeight;
	arrayX = std::vector<double>(width * height, 0.0);
	arrayY = std::vector<double>(width * height, 0.0);
}
PointMap::PointMap(Cell size) {
	PointMap(size.x, size.y);
}
Point PointMap::get(int x, int y) {
	int i = offset(x, y);
	return Point(arrayX[i], arrayY[i]);
}
Point PointMap::get(Cell at) {
	return get(at.x, at.y);
}
void PointMap::set(int x, int y, double px, double py) {
	int i = offset(x, y);
	arrayX[i] = px;
	arrayY[i] = py;
}
void PointMap::set(int x, int y, Point point) {
	set(x, y, point.x, point.y);
}
void PointMap::set(Cell at, Point point) {
	set(at.x, at.y, point);
}
void PointMap::add(int x, int y, double px, double py) {
	int i = offset(x, y);
	arrayX[i] += px;
	arrayY[i] += py;
}
void PointMap::add(int x, int y, Point point) {
	add(x, y, point.x, point.y);
}
void PointMap::add(Cell at, Point point) {
	add(at.x, at.y, point);
}
int PointMap::offset(int x, int y) {
	return y * width + x;
}
