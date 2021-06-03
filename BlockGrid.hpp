#ifndef BLOCKGRID_H
#define BLOCKGRID_H
#include "Block.hpp"
#include "CellGrid.hpp"
class BlockGrid {
	public:
		BlockGrid();
		CellGrid corners;
		BlockGrid(CellGrid localCorners);
		Block get(int x, int y);
		Block get(Cell at);		
};
#endif
