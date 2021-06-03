#include "CellGrid.hpp"
CellGrid::CellGrid::CellGrid() {
}
CellGrid::CellGrid(int width, int height) {
	allX = std::vector<int>(width, 0);
	allY = std::vector<int>(height, 0);
}
CellGrid::CellGrid(Cell size) {
	CellGrid(size.x, size.y);
}
Cell CellGrid::get(int x, int y) {
	return Cell(allX[x], allY[y]);
}
Cell CellGrid::get(Cell at) {
	return get(at.x, at.y);
}
