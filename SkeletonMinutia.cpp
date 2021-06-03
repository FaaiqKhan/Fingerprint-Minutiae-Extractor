#include "SkeletonMinutia.hpp"
SkeletonMinutia::SkeletonMinutia() {
}
SkeletonMinutia::SkeletonMinutia(SkeletonMinutia* skeletonMinutia) {
	position = skeletonMinutia->position;
	considered = skeletonMinutia->considered;
	ridges = skeletonMinutia->ridges;
}
SkeletonMinutia::SkeletonMinutia(Cell localPosition) {
	SkeletonMinutia::position = localPosition;
}
void SkeletonMinutia::attachStart(SkeletonRidge* ridge) {
	if(!contains(ridges, ridge)) {
		ridges.push_back(ridge);
		ridge->start(this);
	}
}
void SkeletonMinutia::detachStart(SkeletonRidge* ridge) {
	std::vector<SkeletonRidge* >::iterator pos;
	int index = 0;
	if (contains(ridges, ridge, &index)) {
		ridges.erase(ridges.begin() + index);
		if (Utils::equals(ridge->start(), this)) {
			ridge->start(nullptr);
		}
	}
}
bool SkeletonMinutia::contains(std::vector<SkeletonRidge* > Ridges, SkeletonRidge* ridge, int* pos) {
	if (Ridges.size() != 0 && ridge != nullptr) {
		for (int i = 0; i < Ridges.size(); ++i) {
			SkeletonRidge* temp = Ridges[i];
			if (temp->endMinutia != nullptr && ridge->endMinutia != nullptr) {
				if (temp->endMinutia->considered == ridge->endMinutia->considered) {
					if (temp->endMinutia->position.x == ridge->endMinutia->position.x && temp->endMinutia->position.y == ridge->endMinutia->position.y) {
						if (temp->endMinutia->ridges.size() == ridge->endMinutia->ridges.size()) {
							if (temp->points->size() == ridge->points->size()) {
								// if (equals(temp->points, ridge->points)) {
									if (temp->startMinutia != nullptr && ridge->startMinutia != nullptr) {
										if (temp->startMinutia->considered == ridge->startMinutia->considered) {
											if (temp->startMinutia->position.x == ridge->startMinutia->position.x && temp->startMinutia->position.y == ridge->startMinutia->position.y) {
												if (temp->startMinutia->ridges.size() == ridge->startMinutia->ridges.size()) {
													*pos = i;
													return true;
												}
											} 
										}
									} else {
										*pos = i;
										return true;
									}
							//	} 
							} 
						} 
					}
				}
				// index++;
			} else {
				if (temp->endMinutia == nullptr && ridge->endMinutia == nullptr) {
					if (temp->startMinutia == nullptr && ridge->startMinutia == nullptr) {
//						if (Utils::equals(temp->points, ridge->points)) {
							*pos = i;
							return true;
//						}
					}
				}
			}
		}
		return false;
	} else {
		return false;
	}
}
bool SkeletonMinutia::contains(std::vector<SkeletonRidge* > Ridges, SkeletonRidge* ridge) {
	for (SkeletonRidge* sRidges : Ridges) {
		if(Utils::equals(sRidges->points, ridge->points)) {
			return true;
		} else {
			return false;
		}
		// compare points list, startMinutia and endMinutia.
	}
	return false;
}
