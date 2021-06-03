#include "Point.hpp"
Point::Point::Point() {
}
Point::Point(double localX, double localY) {
	x = localX;
	y = localY;
}
Point Point::add(Point other) {
	return Point(x + other.x, y + other.y);
}
Point Point::negate() {
	return Point(-x, -y);
}
Point Point::multiply(double factor) {
	return Point(factor * x, factor * y);
}
Cell Point::roundPoint() {
	return Cell((int)round(x), (int)round(y));
}
