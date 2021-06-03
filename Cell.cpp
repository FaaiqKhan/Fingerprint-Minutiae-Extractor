#include "Cell.hpp"
Cell::Cell() {
}
Cell::Cell(Cell* other) {
	x = other->x;
	y = other->y;
}
Cell::Cell(int localX, int localY) {
	x = localX;
	y = localY;
}
int Cell::area() {
	return x * y;
}
int Cell::lengthSq() {
	return Integers::sq(x) + Integers::sq(y);
}
bool Cell::contains(Cell other) {
	return (other.x >= 0 && other.y >= 0 && other.x < x && other.y < y);
}
Cell Cell::plus(Cell other) {
	return Cell(x + other.x, y + other.y);
}
Cell Cell::minus(Cell other) {
	return Cell(x - other.x, y - other.y);
}
Cell Cell::minus(Cell* other) {
	return Cell(this->x - other->x, this->y - other->y);
}
Cell Cell::negate() {
	return Cell(-x, -y);
}
Point Cell::toPoint() {
	return Point(x, y);
}
std::list<Cell> Cell::cornerNeighbors = {
	Cell(-1, -1),
    Cell(0, -1),
    Cell(1, -1),
    Cell(-1, 0),
    Cell(1, 0),
    Cell(-1, 1),
    Cell(0, 1),
    Cell(1, 1)
};
std::vector<Cell> Cell::lineTo(Cell to) {
	std::vector<Cell> result;
	Cell relative = to.minus(this);	
	if (std::abs(relative.x) >= std::abs(relative.y)) {
		result = std::vector<Cell>(std::abs(relative.x) + 1);
		if (relative.x > 0) {
			for (int i = 0; i <= relative.x; ++i) {
				result[i] = Cell(x + i, y + (int)round(i * (relative.y / (double)relative.x)));
			}
		} else if (relative.x < 0) {
			for (int i = 0; i <= -relative.x; ++i) {
				result[i] = Cell(x - i, y - (int)round(i * (relative.y / (double)relative.x)));
			}
		} else {
			result[0] = Cell(this->x, this->y);
		}
	} else {
		result = std::vector<Cell>(std::abs(relative.y) + 1);
		if (relative.y > 0) {
			for (int i = 0; i <= relative.y; ++i) {
				result[i] = Cell(x + (int)round(i * (relative.x / (double)relative.y)), y + i);
			}
		} else if (relative.y < 0) {
			for (int i = 0; i <= - relative.y; ++i) {
				result[i] = Cell(x - (int)round(i * (relative.x / (double)relative.y)), y - i);
			}
		} else {
			result[0] = Cell(this->x, this->y);
		}
	}
	return result;
}
