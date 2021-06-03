#ifndef POINT_H
#define POINT_H
#include "Cell.hpp"
#include "cmath"
class Cell;
class Point {
	public:
		double x;
		double y;
		Point();
		Point(double localX, double localY);
		Point add(Point ohter);
		Point negate();
		Point multiply(double factor);
		Cell roundPoint();
};
#endif
