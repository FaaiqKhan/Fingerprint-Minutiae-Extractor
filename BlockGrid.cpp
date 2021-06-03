#include "BlockGrid.hpp"
BlockGrid::BlockGrid::BlockGrid() {
}
BlockGrid::BlockGrid(CellGrid localCorners) {
	corners = localCorners;
}
Block BlockGrid::get(Cell at) {
	return get(at.x, at.y);
}
Block BlockGrid::get(int x, int y) {
	return Block::between(corners.get(x, y), corners.get(x + 1, y + 1));
}
