#include "Gap.hpp"
bool operator < (const Gap& gap1, const Gap& gap2) {
	return ((gap1.end1->ridges.size() < gap2.end1->ridges.size()) && (gap1.end2->ridges.size() < gap2.end2->ridges.size()));
}
