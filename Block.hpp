#include "Cell.hpp"
#include <cmath>
class Block {
	public:
		int x;
		int y;
		int width;
		int height;
		Block(Cell size);
		Block(int localX, int localY, int localWidth, int localHeight);
		int left();
		int bottom();
		int right();
		int top();
		int area();
		static Block between(int startX, int startY, int endX, int endY);
		static Block between(Cell start, Cell end);
		static Block around(int localX, int localY, int radius);
		static Block around(Cell center, int radius);
		Cell center();
		Block interset(Block other);
		Block move(Cell delta);		
};
