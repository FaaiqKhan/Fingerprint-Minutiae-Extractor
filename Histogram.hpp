#include <cmath>
#include <vector>
#include "Cell.hpp"
class Histogram {
	public:
		int width;
		int height;
		int depth;
		std::vector<int> array;
		Histogram();
		Histogram(int localWidth, int localHeight, int localDepth);
		int constrain(int z);
		int get(int x, int y, int z);
		int get(Cell at, int z);
		int sum(int x, int y);
		int sum(Cell at);
		void set(int x, int y, int z, int value);
		void set(Cell at, int z, int value);
		void add(int x, int y, int z, int value);
		void add(Cell at, int z, int value);
		void increment(int x, int y, int z);
		void increment(Cell at, int z);
	private:
		int offset(int x, int y, int z);
};
