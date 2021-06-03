#include <stdio.h>
#include "Cell.hpp"
#include "Integers.hpp"
#include "CellGrid.hpp"
#include "BlockGrid.hpp"
class BlockMap {
	public:
		Cell pixelCount;
		Cell blockCount;
		Cell cornerCount;
		CellGrid corners;
		BlockGrid blockAreas;
		CellGrid blockCenters;
		BlockGrid cornerAreas;
		BlockMap(int width, int height, int maxBlockSize);
		CellGrid initCorners();
		CellGrid initBlockCenters();
		BlockGrid initCornerAreas();
};
