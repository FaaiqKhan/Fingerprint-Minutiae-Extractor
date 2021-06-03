#ifndef SKELETONMINUTIA_H
#define SKELETONMINUTIA_H
#include <list>
#include <vector>
#include "Cell.hpp"
#include "Utils.hpp"
#include "SkeletonRidge.hpp"
class SkeletonMinutia {
	public:
		Cell position;
		bool considered = true;
		std::vector<SkeletonRidge* > ridges;
		SkeletonMinutia();
		SkeletonMinutia(SkeletonMinutia*);
		SkeletonMinutia(Cell localPosition);
		void attachStart(SkeletonRidge* ridge);
		void detachStart(SkeletonRidge* ridge);
		bool contains(std::vector<SkeletonRidge* > ridges, SkeletonRidge* ridge);
		bool contains(std::vector<SkeletonRidge* > ridges, SkeletonRidge* ridge, int* pos);
};
#endif
