#ifndef GAP_H
#define GAP_H
#include "Integers.hpp"
#include "SkeletonMinutia.hpp"
class Gap {
	public:
		int distance;
		SkeletonMinutia* end1;
		SkeletonMinutia* end2;
};
bool operator < (const Gap& gap1, const Gap& gap2);
#endif
