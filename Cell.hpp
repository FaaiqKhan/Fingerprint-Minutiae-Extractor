#ifndef CELL_H
#define CELL_H
#include "Point.hpp"
#include "Integers.hpp"
#include <list>
#include <vector>
class Point;
class Cell {
	public:
		int x;
		int y;
		Cell();
		Cell(Cell* other);
		Cell(int localX, int localY);	
		int area();
		int lengthSq();
		bool contains(Cell);
		Cell plus(Cell);
		Cell minus(Cell);
		Cell minus(Cell*);
		Cell negate();
		Point toPoint();
		static std::list<Cell> cornerNeighbors;
		std::vector<Cell> lineTo(Cell to);
		// Overloading operator is to insert data into map.
		bool operator<(const Cell& pos) const {
			return ((this->x < pos.x && this->y < pos.y) || (this->x < pos.x && this->y == pos.y) || (this->x == pos.x && this->y < pos.y) || (this->x < pos.x && this->y > pos.y) || (this->x > pos.x && this->y < pos.y));
    	}
    	// Overloading operator for list for comparasion.
    	bool operator==(const Cell& pos) const {
			return (this->x == pos.x && this->y == pos.y);
		}
};
#endif
