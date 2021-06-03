#ifndef BOOLEANMAP_H
#define BOOLEANMAP_H
#include <vector>
#include <stdio.h>
#include <iostream>
#include "Cell.hpp"
class BooleanMap {
	public:
		int width;
		int height;
		std::vector<bool> array;
		BooleanMap();
		BooleanMap(Cell localSize);
		BooleanMap(BooleanMap* localOther);
		BooleanMap(int localWidth, int localHeight);
		Cell size();
		bool get(int x, int y);
		bool get(Cell at);
		bool get(int x, int y, bool fallback);
		bool get(Cell at, bool fallback);
		void set(int x, int y, bool value);
		void set(Cell at, bool value);
		void invert();
		void merge(BooleanMap other);
	private:
		int offset(int x, int y);
};
#endif
