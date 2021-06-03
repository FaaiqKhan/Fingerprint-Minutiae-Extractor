#include "SkeletonRidge.hpp"
SkeletonRidge::SkeletonRidge() {
	points = new std::list<Cell>();
	reversed = new SkeletonRidge(this);
}
SkeletonRidge::SkeletonRidge(SkeletonRidge* reverse) {
	this->points = new std::list<Cell>();
	this->reversed = reverse;
}
SkeletonMinutia* SkeletonRidge::start() {
	return startMinutia;
}
void SkeletonRidge::start(SkeletonMinutia* value) {
	if (!equal(startMinutia, value)) {
		if (startMinutia != nullptr) {
			SkeletonMinutia* detachFrom = nullptr;
			detachFrom = startMinutia;
			startMinutia = nullptr;
			detachFrom->detachStart(this);
		}
		startMinutia = value;
		if (startMinutia != nullptr) {
			startMinutia->attachStart(this);
		}
		reversed->endMinutia = value;
	}
}
SkeletonMinutia* SkeletonRidge::end() {
	return endMinutia;
}
void SkeletonRidge::end(SkeletonMinutia* value) {
	if (!equal(endMinutia, value)) {
		endMinutia = value;
		reversed->start(value);
	}
}
void SkeletonRidge::detach() {
	start(nullptr); // Checked.
	end(nullptr); // Checked.
}
bool SkeletonRidge::equal(SkeletonMinutia* p1, SkeletonMinutia* p2) {
	if (p1 != nullptr && p2 != nullptr) {
		if (p1->considered == p2->considered) {
			if (p1->position.x == p2->position.x && p1->position.y == p2->position.y) {
				if (p1->ridges.size() == p2->ridges.size()) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}
double SkeletonRidge::direction() {
	int first = FingerprintContext::ridgeDirectionSkip;
	int last = FingerprintContext::ridgeDirectionSkip + FingerprintContext::ridgeDirectionSample - 1;
	if (last >= points->size()) {
		int shift = last - points->size() + 1;
		last -= shift;
		first -= shift;
	}
	if (first < 0) {
		first = 0;
	}
	std::list<Cell>::iterator itr1 = points->begin();
	std::list<Cell>::iterator itr2 = points->begin();
	std::advance(itr1, first);
	std::advance(itr2, last);
	return Angle::atan(*itr1, *itr2);
}
