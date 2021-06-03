#include <vector>
#include <stdio.h>
#include "Cell.hpp"
class DoubleMap {
	private:
		int offset(int x, int y);
	public:
		int width;
		int height;
		std::vector<double> array;
		DoubleMap(int localWidth, int localHeight);
		DoubleMap(Cell size);
		Cell size();
		double get(int x, int y);
		double get(Cell at);
		void set(int x, int y, double value);
		void set(Cell at, double value);
		void add(int x, int y, double value);
		void add(Cell at, double value);
		void multiply(int x, int y, double value);
		void multiply(Cell at, double value);
};
