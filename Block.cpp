#include "Block.hpp"
Block::Block(Cell size) {
	Block(0, 0, size.x, size.y);
}
Block::Block(int localX, int localY, int localWidth, int localHeight) {
	x = localX;
	y = localY;
	width = localWidth;
	height = localHeight;
}
int Block::left() {
	return x;
}
int Block::bottom() {
	return y;
}
int Block::right() {
	return x + width;
}
int Block::top() {
	return y + height;
}
int Block::area() {
	return width * height;
}
Block Block::between(int startX, int startY, int endX, int endY) {
	return Block(startX, startY, endX - startX, endY - startY);
}
Block Block::between(Cell start, Cell end) {
	return between(start.x, start.y, end.x, end.y);
}
Block Block::around(int localX, int localY, int radius) {
	return between(localX - radius, localY - radius, localX + radius + 1, localY + radius + 1);
}
Block Block::around(Cell center, int radius) {
	return around(center.x, center.y, radius);
}
Cell Block::center() {
	return Cell((right() + left()) / 2, (bottom() + top()) / 2);
}
Block Block::interset(Block other) {
	return between(Cell(fmax(left(), other.left()), fmax(bottom(), other.bottom())), Cell(fmin(right(), other.right()), fmin(top(), other.top())));
}
Block Block::move(Cell delta) {
	return Block(x + delta.x, y + delta.y, width, height);
}
