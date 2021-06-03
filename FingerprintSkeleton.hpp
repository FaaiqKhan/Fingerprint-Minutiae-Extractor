#ifndef FINGERPRINTSKELETON_H
#define FINGERPRINTSKELETON_H
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Gap.hpp"
#include "Cell.hpp"
#include "Utils.hpp"
#include "Angle.hpp"
#include "BooleanMap.hpp"
#include "SkeletonRidge.hpp"
#include "SkeletonMinutia.hpp"
#include "FingerprintContext.hpp"
class FingerprintSkeleton {
	private:
		enum NeighborhoodType {
			Skeleton,
			Ending,
			Removable
		};
		static std::list<Cell>* addAll(std::list<Cell>* list);
		static std::list<Cell>* addAll(std::list<Cell>* list, std::list<Cell>* obj);
		static std::list<Cell> getList(std::list<Cell>* list);
		static std::vector<NeighborhoodType> neighborhoodTypes();
		static std::map<Cell, std::list<Cell>* > linkNeighboringMinutiae(std::list<Cell> minutiae);
		SkeletonMinutia* get(std::map<Cell, SkeletonMinutia* > map, Cell key);
		static void addGapRidge(BooleanMap shadow, Gap gap, std::vector<Cell> line);
	public:
		Cell size;
		std::list<SkeletonMinutia*> minutiae;
		BooleanMap shadow();
		BooleanMap thin(BooleanMap input);
		FingerprintSkeleton(BooleanMap binary);
		std::list<Cell> findMinutiae(BooleanMap thinned);
		std::map<Cell, SkeletonMinutia* > minutiaCenters(std::map<Cell, std::list<Cell>* > linking);
		Cell angleSampleForGapRemoval(SkeletonMinutia* minutia);
		void filter();
		void removeDots();
		void removeGaps();
		void removeKnots();
		void removeTails();
		void removePores();
		void fixLinkingGaps();
		void removeFragments();
		void disableBranchMinutiae();
		void removeMinutia(SkeletonMinutia* minutia);
		void addMinutia(SkeletonMinutia* minutia);
		void traceRidges(BooleanMap thinned, std::map<Cell, SkeletonMinutia* > minutiaePoints);
		bool isWithinGapLimits(SkeletonMinutia* end1, SkeletonMinutia* end2);
		bool isRidgeOverlapping(std::vector<Cell> line, BooleanMap shadow);
};
#endif
