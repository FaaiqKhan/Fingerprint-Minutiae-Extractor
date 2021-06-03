#ifndef CELLGRID_H
#define CELLGRID_H
#include <vector>
#include "Cell.hpp"
class CellGrid {
	public:
		CellGrid();
		std::vector<int> allX;
		std::vector<int> allY;
		CellGrid(Cell size);
		CellGrid(int width, int height);
		Cell get(int x, int y);
		Cell get(Cell at);
};
#endif
