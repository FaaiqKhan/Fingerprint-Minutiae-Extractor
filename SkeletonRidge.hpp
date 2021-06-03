#ifndef SKELETONRIDGE_H
#define SKELETONRIDGE_H
#include <vector>
#include <iostream>
#include <algorithm>
#include "Cell.hpp"
#include "Angle.hpp"
#include "FingerprintContext.hpp"
class SkeletonMinutia;
class SkeletonRidge {
	private:
		static bool equal(SkeletonMinutia* p1, SkeletonMinutia* p2);
	public:
		SkeletonMinutia* startMinutia = nullptr;
		SkeletonMinutia* endMinutia = nullptr;
		std::list<Cell>* points;
		SkeletonRidge* reversed;
		SkeletonRidge();
		SkeletonRidge(SkeletonRidge* reverse);
		SkeletonMinutia* end();
		SkeletonMinutia* start();
		void detach();
		void end(SkeletonMinutia* value);
		void start(SkeletonMinutia* value);
		double direction();
		bool operator==(const SkeletonRidge& skeletonRidge) const {
			std::list<Cell>* temp1 = this->points;
			std::list<Cell>* temp2 = skeletonRidge.points;
			int sizeOfList = temp1->size();
			if (sizeOfList == temp2->size()) {
				int matchingCount = 0;
				std::list<Cell>::iterator itr1 = temp1->begin();
				std::list<Cell>::iterator itr2 = temp2->begin();
				for (int i = 1; i < sizeOfList; ++i) {
					if (itr1->x == itr2->x && itr1->y == itr2->y) {
						matchingCount++;
						std::advance(itr1, i);
						std::advance(itr2, i);
					} else {
						return false;
					}
				}
				if (matchingCount == sizeOfList) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
};
#include "SkeletonMinutia.hpp"
#endif
