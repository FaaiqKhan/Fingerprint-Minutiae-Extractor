#include "BlockMap.hpp"
BlockMap::BlockMap(int width, int height, int maxBlockSize) {
	pixelCount = Cell(width, height);
	blockCount = Cell(Integers::roundUpDiv(pixelCount.x, maxBlockSize), Integers::roundUpDiv(pixelCount.y, maxBlockSize));
	cornerCount = Cell(blockCount.x + 1, blockCount.y + 1);
	corners = initCorners();
	blockAreas = BlockGrid(corners);
	blockCenters = initBlockCenters();
	cornerAreas = initCornerAreas(); 
}
CellGrid BlockMap::initCorners() {
	CellGrid grid = CellGrid(cornerCount);
	for(int y = 0; y < cornerCount.y; ++y) {
		grid.allY.push_back(y * pixelCount.y / blockCount.y);
	}
	for(int x = 0; x < cornerCount.x; ++x) {
		grid.allX.push_back(x * pixelCount.x / blockCount.x);
	}
	return grid;
}
CellGrid BlockMap::initBlockCenters() {
	CellGrid grid = CellGrid(blockCount);
	for(int y = 0; y < blockCount.y; ++y) {
		grid.allY.push_back(blockAreas.get(0, y).center().y);
	}
	for(int x = 0; x < blockCount.x; ++x) {
		grid.allX.push_back(blockAreas.get(x, 0).center().x);
	}
	return grid;
}
BlockGrid BlockMap::initCornerAreas() {
	CellGrid grid = CellGrid(cornerCount.x + 1, cornerCount.y + 1);
	grid.allY[0] = 0;
	for(std::vector<int>::size_type y = 0; y < blockCount.y; ++y) {
		grid.allY[y + 1] = blockCenters.get(0, y).y;
	}
	grid.allY[blockCount.y + 1] = pixelCount.y;
	grid.allX[0] = 0;
	for(int x = 0; x < blockCount.x; ++x) {
		grid.allX[x + 1] = blockCenters.get(x, 0).x;
	}
	grid.allX[blockCount.x + 1] = pixelCount.x;
	return BlockGrid(grid);
}
