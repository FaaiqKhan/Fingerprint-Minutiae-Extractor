#ifndef PointMap_H
#define PointMap_H
#include <vector>
#include <iostream>
#include "Cell.hpp"
#include "Point.hpp"
class PointMap {
	public:
		int width;
		int height;
		std::vector<double> arrayX;
		std::vector<double> arrayY;
		PointMap(int localWidth, int localHeight);
		PointMap(Cell size);
		Point get(int x, int y);
		Point get(Cell at);
		void set(Cell at, Point point);
		void set(int x, int y, double px, double py);
		void set(int x, int y, Point point);
		void add(int x, int y, double px, double py);
		void add(int x, int y, Point point);
		void add(Cell at, Point point);
		int offset(int x, int y);
};
#endif
