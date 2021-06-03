#include "FingerprintSkeleton.hpp"
FingerprintSkeleton::FingerprintSkeleton(BooleanMap binary) {
	size = binary.size(); // Checked.
	// std::cout<<"size initialized."<<std::endl;
	BooleanMap thinned = thin(binary); // Checked.
	// std::cout<<"thinned"<<std::endl;
	std::list<Cell> minutiaPoints = findMinutiae(thinned); // Checked.
	// std::cout<<"minutiaPoints"<<std::endl;
	std::map<Cell, std::list<Cell>* > linking = linkNeighboringMinutiae(minutiaPoints); // Checked.
	// std::cout<<"linking"<<std::endl;
	std::map<Cell, SkeletonMinutia* > minutiaMap = minutiaCenters(linking); // Checked.
	// std::cout<<"minutiaMap"<<std::endl;
	traceRidges(thinned, minutiaMap); // Checked.
	// std::cout<<"traceRidges"<<std::endl;
	fixLinkingGaps(); // Checked.
	// std::cout<<"fixLinkingGaps"<<std::endl;
	
	/*std::cout<<"C++ Logs"<<std::endl;
	std::cout<<"size of minutiae: "<<minutiae.size()<<std::endl;
	for (SkeletonMinutia* minutia : minutiae) {
		std::cout<<"["<<minutia->position.x<<","<<minutia->position.y<<"]*"<<minutia->ridges.size()<<std::endl;
		for (SkeletonRidge* ridge : minutia->ridges) {
			std::cout<<"endMinutia = ["<<ridge->endMinutia->position.x<<","<<ridge->endMinutia->position.y<<"]*"<<ridge->endMinutia->ridges.size()<<std::endl;
			std::cout<<"point size = "<<ridge->points->size()<<std::endl;		
			std::cout<<"startMinutia = ["<<ridge->startMinutia->position.x<<","<<ridge->startMinutia->position.y<<"]*"<<ridge->startMinutia->ridges.size()<<std::endl;	
			std::cout<<"reversed"<<std::endl;
			std::cout<<"endMinutia = ["<<ridge->reversed->endMinutia->position.x<<","<<ridge->reversed->endMinutia->position.y<<"]*"<<ridge->reversed->endMinutia->ridges.size()<<std::endl;
			std::cout<<"point size = "<<ridge->reversed->points->size()<<std::endl;		
			std::cout<<"startMinutia = ["<<ridge->reversed->startMinutia->position.x<<","<<ridge->reversed->startMinutia->position.y<<"]*"<<ridge->reversed->startMinutia->ridges.size()<<std::endl;	
		}
	}*/
	
	filter();
	// std::cout<<"filter"<<std::endl;
}
std::vector<FingerprintSkeleton::NeighborhoodType> FingerprintSkeleton::neighborhoodTypes() {
	std::vector<FingerprintSkeleton::NeighborhoodType> types(256);
	for(int mask = 0; mask < 256; ++mask) {
		bool TL = (mask & 1) != 0;
		bool TC = (mask & 2) != 0;
		bool TR = (mask & 4) != 0;
		bool CL = (mask & 8) != 0;
		bool CR = (mask & 16) != 0;
		bool BL = (mask & 32) != 0;
		bool BC = (mask & 64) != 0;
		bool BR = (mask & 128) != 0;
		int count = Integers::bitCount(mask);
		bool diagonal = (!TC && !CL && TL) || (!CL && !BC && BL) || (!BC && !CR && BR) || (!CR && !TC && TR);
		bool horizontal = horizontal = !TC && !BC && (TR || CR || BR) && (TL || CL || BL);
		bool vertical = !CL && !CR && (TL || TC || TR) && (BL || BC || BR);
		bool end = (count == 1);
		if (end) {
			types[mask] = FingerprintSkeleton::NeighborhoodType::Ending;
		} else if (!diagonal && ! horizontal && !vertical) {
			types[mask] = FingerprintSkeleton::NeighborhoodType::Removable;
		} else {
			types[mask] = FingerprintSkeleton::NeighborhoodType::Skeleton;
		}
	}
	return types;
}

BooleanMap FingerprintSkeleton::thin(BooleanMap input) {
	std::vector<NeighborhoodType> neighborhoodTypes = FingerprintSkeleton::neighborhoodTypes();
	BooleanMap partial = BooleanMap(size.x, size.y);
	for (int y = 1; y < size.y - 1; ++y) {
		for (int x = 1; x < size.x - 1; ++x) {
			partial.set(x, y, input.get(x, y));
		}
	}
	BooleanMap thinned = BooleanMap(size.x, size.y);
	bool removedAnything = true;
	for (int i = 0; i < 26 && removedAnything; ++i) {
		removedAnything = false;
		for (int evenY = 0; evenY < 2; ++evenY) {
			for (int evenX = 0; evenX < 2; ++evenX) {
				for (int y = 1 + evenY; y < size.y - 1; y += 2) {
					for (int x = 1 + evenX; x < size.x - 1; x += 2) {
						if (partial.get(x, y) && !thinned.get(x, y) && !(partial.get(x, y - 1) && partial.get(x, y + 1) && partial.get(x - 1, y) && partial.get(x + 1, y))) {
							int neighbors = (partial.get(x + 1, y + 1) ? 128 : 0) | (partial.get(x, y + 1) ? 64 : 0) | (partial.get(x - 1, y + 1) ? 32 : 0) | (partial.get(x + 1, y) ? 16 : 0) | (partial.get(x - 1, y) ? 8 : 0) | (partial.get(x + 1, y - 1) ? 4 : 0) | (partial.get(x, y - 1) ? 2 : 0) | (partial.get(x - 1, y - 1) ? 1 : 0);
							if (neighborhoodTypes[neighbors] == NeighborhoodType::Removable || (neighborhoodTypes[neighbors] == NeighborhoodType::Ending && Utils::isFalseEnding(partial, Cell(x, y)))) {
								removedAnything = true;
								partial.set(x, y, false);
							} else {
								thinned.set(x, y, true);
							}
						}
					}
				}
			}
		}
	}
	return thinned;
}
std::list<Cell> FingerprintSkeleton::findMinutiae(BooleanMap thinned) {
	std::list<Cell> result;
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			Cell at = Cell(x, y);
			if (thinned.get(at)) {
				int count = 0;
				for (Cell relative : Cell::cornerNeighbors) {
					if (thinned.get(at.plus(relative), false)) {
						++count;
					}
				}
				if (count == 1 || count > 2) {
					result.push_back(at);
				}
			}
		}
	}
	return result;
}
std::list<Cell>* FingerprintSkeleton::addAll(std::list<Cell>* list) {
	std::list<Cell>* localVar = new std::list<Cell>();
	for (Cell cell : *list) {
		localVar->push_back(cell);
	}
	return localVar;
}
std::list<Cell>* FingerprintSkeleton::addAll(std::list<Cell>* list, std::list<Cell>* obj) {
	std::list<Cell>* localVar = new std::list<Cell>();
	for (Cell cell : *obj)
		localVar->push_back(cell);
	for (Cell cell : *list)
		localVar->push_back(cell);
	return localVar;
}
SkeletonMinutia* FingerprintSkeleton::get(std::map<Cell, SkeletonMinutia* > map, Cell key) {
	std::map<Cell, SkeletonMinutia* >::iterator itr;
	for (itr = map.begin(); itr != map.end(); itr++) {
		Cell cell = itr->first;
		if (cell.x == key.x && cell.y == key.y) {
			return itr->second;
		}
	}
	return nullptr;
}
std::map<Cell, std::list<Cell>* > FingerprintSkeleton::linkNeighboringMinutiae(std::list<Cell> minutiae) {
	std::map<Cell, std::list<Cell>* > linking;
	for (Cell minutiaPos : minutiae) {
		std::list<Cell>* ownLinks = nullptr;
		for (Cell neighborRelative : Cell::cornerNeighbors) {
			Cell neighborPos = minutiaPos.plus(neighborRelative);
			if (Utils::containsKey(linking, neighborPos)) {
				std::list<Cell>* neighborLinks = Utils::get(linking, neighborPos);				
				if (!Utils::equals(neighborLinks, ownLinks)) {
					if (ownLinks != nullptr) {
						neighborLinks = addAll(ownLinks, neighborLinks);
						for (const Cell & mergedPos : *ownLinks) { 
							linking.insert(std::pair<Cell, std::list<Cell>* >(mergedPos, neighborLinks)); 
						}
					}
					ownLinks = neighborLinks;
				}
			}
		}
		if (ownLinks == nullptr) {
			ownLinks = new std::list<Cell>;
		}
		ownLinks->push_back(minutiaPos);
		linking.insert(std::pair<Cell, std::list<Cell>* >(minutiaPos, ownLinks));
	}
	return linking;
}
std::map<Cell, SkeletonMinutia* > FingerprintSkeleton::minutiaCenters(std::map<Cell, std::list<Cell>* > linking) {
	std::map<Cell, SkeletonMinutia* > centers;
	std::map<Cell, std::list<Cell>* >::iterator itr;
	for (itr = linking.begin(); itr != linking.end(); itr++) {
		Cell currentPos = itr->first;
		std::list<Cell>* linkedMinutiae = Utils::get(linking, currentPos);
		Cell primaryPos = linkedMinutiae->front();
		if (!Utils::containsKey(centers, primaryPos)) {
			Cell sum = Cell(0, 0);
			for (Cell linkedPos : *linkedMinutiae) {
				sum = sum.plus(linkedPos);
			}
			Cell center = Cell(sum.x / linkedMinutiae->size(), sum.y / linkedMinutiae->size());
			SkeletonMinutia* minutia = new SkeletonMinutia(center);
			addMinutia(minutia);
			centers.insert(std::pair<Cell, SkeletonMinutia* >(primaryPos, minutia));
		}
		centers.insert(std::pair<Cell, SkeletonMinutia* >(currentPos, get(centers, primaryPos)));
	}
	return centers;
}
void FingerprintSkeleton::traceRidges(BooleanMap thinned, std::map<Cell, SkeletonMinutia* > minutiaePoints) {
	std::map<Cell, SkeletonRidge* > leads;
	std::map<Cell, SkeletonMinutia* >::iterator itr;
	for (itr = minutiaePoints.begin(); itr != minutiaePoints.end(); ++itr) {
		Cell minutiaPoint = itr->first;
		for (Cell startRelative : Cell::cornerNeighbors) {
			Cell start = minutiaPoint.plus(startRelative);
			if (thinned.get(start, false) && !Utils::containsKey(minutiaePoints, start) && !Utils::containsKey(leads, start)) {
				SkeletonRidge* ridge = new SkeletonRidge();
				ridge->points->push_back(minutiaPoint);
				ridge->points->push_back(start);
				Cell previous = minutiaPoint;
				Cell current = start;
				do {
					Cell next = Cell(0, 0);
					for (Cell nextRelative : Cell::cornerNeighbors) {
						next = current.plus(nextRelative);
						if (thinned.get(next.x, next.y, false) && !Utils::equals(next, previous)) {
							break;
						}
					}
					previous = current;
					current = next;
					ridge->points->push_back(current);
				} while (!Utils::containsKey(minutiaePoints, current));
				for (Cell cell : *ridge->points) {
					ridge->reversed->points->push_front(cell);
				}
				Cell end = current;
				ridge->start(get(minutiaePoints, minutiaPoint));
				ridge->end(get(minutiaePoints, end));
				std::list<Cell>::iterator forwardItr = ridge->points->begin();
				std::list<Cell>::iterator reverseItr = ridge->reversed->points->begin();
				std::advance(forwardItr, 1);
				std::advance(reverseItr, 1);
				leads.insert(std::pair<Cell, SkeletonRidge* >(Cell(forwardItr->x, forwardItr->y), ridge));
				leads.insert(std::pair<Cell, SkeletonRidge* >(Cell(reverseItr->x, reverseItr->y), ridge));
			}
		}
	}
}
void FingerprintSkeleton::fixLinkingGaps() {
	for (SkeletonMinutia* minutia : minutiae) {
		for (SkeletonRidge* ridge : minutia->ridges) {
			if (!Utils::equals(ridge->points->front(), minutia->position)) {
				std::vector<Cell> filling = ridge->points->front().lineTo(minutia->position);
				for (int i = 1; i < filling.size(); ++i) {
					ridge->points->push_front(filling[i]);
					ridge->reversed->points->push_back(filling[i]);
				}
			}
		}
	}
}
void FingerprintSkeleton::filter() {
	removeDots(); // Checked.
	std::cout<<"removeDots"<<std::endl;
	removePores(); // Checked.
	std::cout<<"removePores"<<std::endl;
	removeGaps(); // Checked.
	std::cout<<"removeGaps"<<std::endl;
	removeTails();
	std::cout<<"removeTails"<<std::endl;
	removeFragments();
	std::cout<<"removeFragments"<<std::endl;
	disableBranchMinutiae();
	std::cout<<"disableBranchMinutiae"<<std::endl;
}
void FingerprintSkeleton::removeDots() {
	std::list<SkeletonMinutia* > removed;
	for (SkeletonMinutia* minutia : minutiae) {
		if (minutia->ridges.empty()) {
			removed.push_back(minutia);
		}
	}
	for (SkeletonMinutia* minutia : removed) {
		removeMinutia(minutia);
	}
}
void FingerprintSkeleton::removePores() {
	for (SkeletonMinutia* minutia : minutiae) {
		if (minutia->ridges.size() == 3) {
			for (int exit = 0; exit < 3; ++exit) {
				SkeletonRidge* exitRidge = minutia->ridges[exit];
				SkeletonRidge* arm1 = minutia->ridges[(exit + 1) % 3];
				SkeletonRidge* arm2 = minutia->ridges[(exit + 2) % 3];
				if (arm1->end() == arm2->end() && exitRidge->end() != arm1->end() 
					&& arm1->end() != minutia && exitRidge->end() != minutia) {
					SkeletonMinutia* end = arm1->end();
					if (end->ridges.size() == 3 && arm1->points->size() <= FingerprintContext::maxPoreArm && arm2->points->size() <= FingerprintContext::maxPoreArm) {
						arm1->detach();
						arm2->detach();
						SkeletonRidge* merged =  new SkeletonRidge();
						merged->start(minutia);
						merged->end(end);
						for (Cell point : minutia->position.lineTo(end->position)) {
							merged->points->push_back(point);
							merged->reversed->points->push_front(point);
						}
					}
					break;
				}
			}
		}
	}
	removeKnots();
}
void FingerprintSkeleton::removeGaps() {
	std::priority_queue<Gap> queue;
	for (SkeletonMinutia* end1 : minutiae) {
		if (end1->ridges.size() == 1 && end1->ridges[0]->points->size() >= FingerprintContext::shortestJoinedEnding) {
			for (SkeletonMinutia* end2 : minutiae) {
				if (!Utils::equals(end2, end1) && end2->ridges.size() == 1 && !Utils::equals(end1->ridges[0]->end(), end2) && end2->ridges[0]->points->size() >= FingerprintContext::shortestJoinedEnding && isWithinGapLimits(end1, end2)) {
					Gap gap;
					gap.distance = end1->position.minus(end2->position).lengthSq();
					gap.end1 = end1;
					gap.end2 = end2;
					queue.push(gap);
				}
			}
		}
	}
	std::cout<<"removeGaps first loop completed."<<std::endl;
	BooleanMap shadoW = shadow();
	while (!queue.empty()) {
		Gap gap = queue.top();
		queue.pop();
		if (gap.end1->ridges.size() == 1 && gap.end2->ridges.size() == 1) {
			std::vector<Cell> line = gap.end1->position.lineTo(gap.end2->position);
			if (!isRidgeOverlapping(line, shadoW)) {
				addGapRidge(shadoW, gap, line);
			}
		}
	}
	std::cout<<"removeGaps second loop completed."<<std::endl;
	removeKnots();
}
bool FingerprintSkeleton::isWithinGapLimits(SkeletonMinutia* end1, SkeletonMinutia* end2) {
	int distanceSq = end1->position.minus(end2->position).lengthSq();
	if (distanceSq <= Integers::sq(FingerprintContext::maxRuptureSize)) {
		return true;
	}
	if (distanceSq > Integers::sq(FingerprintContext::maxGapSize)) {
		return false;
	}
	double gapDirection = Angle::atan(end1->position, end2->position);
	double direction1 = Angle::atan(end1->position, angleSampleForGapRemoval(end1));
	if (Angle::distance(direction1, Angle::opposite(gapDirection)) > FingerprintContext::maxGapAngle) {
		return false;	
	}
	double direction2 = Angle::atan(end2->position, angleSampleForGapRemoval(end2));
	if (Angle::distance(direction2, gapDirection) > FingerprintContext::maxGapAngle) {
		return false;
	}	
	return true;
}
Cell FingerprintSkeleton::angleSampleForGapRemoval(SkeletonMinutia* minutia) {
	SkeletonRidge* ridge = minutia->ridges[0];
	if (FingerprintContext::gapAngleOffset < ridge->points->size()) {
		std::list<Cell>::iterator itr = ridge->points->begin();
		std::advance(itr, FingerprintContext::gapAngleOffset);
		return Cell(itr->x, itr->y);
	} else {
		return ridge->end()->position;
	}
}
bool FingerprintSkeleton::isRidgeOverlapping(std::vector<Cell> line, BooleanMap shadow) {
	for (int i = FingerprintContext::toleratedGapOverlap; i < (int)(line.size()) - FingerprintContext::toleratedGapOverlap; ++i)
		if (shadow.get(line[i].x, line[i].y))
			return true;
	return false;
}
void FingerprintSkeleton::addGapRidge(BooleanMap shadow, Gap gap, std::vector<Cell> line) {
	SkeletonRidge* ridge = new SkeletonRidge;
	for (Cell point : line) {
		ridge->points->push_back(point);
		ridge->reversed->points->push_front(point);
	}
	ridge->start(gap.end1);
	ridge->end(gap.end2);
	for (Cell point : line) {
		shadow.set(point.x, point.y, true);
	}
}
void FingerprintSkeleton::removeTails() {
//	int index = 0;
//	std::cout<<"size of minutiae: "<<minutiae.size()<<std::endl;
//	for (SkeletonMinutia* minutia : minutiae) {
//		std::cout<<"index: "<<index<<"["<<minutia->position.x<<","<<minutia->position.y<<"]*"<<minutia->ridges.size()<<std::endl;
//		for (SkeletonRidge* ridge : minutia->ridges) {
//			std::cout<<"endMinutia = ["<<ridge->endMinutia->position.x<<","<<ridge->endMinutia->position.y<<"]*"<<ridge->endMinutia->ridges.size()<<std::endl;
//			std::cout<<"point size = "<<ridge->points->size()<<std::endl;		
//			std::cout<<"startMinutia = ["<<ridge->startMinutia->position.x<<","<<ridge->startMinutia->position.y<<"]*"<<ridge->startMinutia->ridges.size()<<std::endl;	
//			std::cout<<"reversed"<<std::endl;
//			std::cout<<"endMinutia = ["<<ridge->reversed->endMinutia->position.x<<","<<ridge->reversed->endMinutia->position.y<<"]*"<<ridge->reversed->endMinutia->ridges.size()<<std::endl;
//			std::cout<<"point size = "<<ridge->reversed->points->size()<<std::endl;		
//			std::cout<<"startMinutia = ["<<ridge->reversed->startMinutia->position.x<<","<<ridge->reversed->startMinutia->position.y<<"]*"<<ridge->reversed->startMinutia->ridges.size()<<std::endl;	
//		}
//		index++;
//	}
	for (SkeletonMinutia* minutia : minutiae) {
		if (minutia->ridges.size() == 1) {
			if (minutia->ridges[0]->end()->ridges.size() >= 3) {
				if (minutia->ridges[0]->points->size() < FingerprintContext::minTailLength) {
					minutia->ridges[0]->detach();
				}
			}
		}
	}
	removeDots();
	removeKnots();
}
void FingerprintSkeleton::removeFragments() {
	for (SkeletonMinutia* minutia : minutiae) {
		if (minutia->ridges.size() == 1) {
			SkeletonRidge* ridge = minutia->ridges[0];
			if (ridge->end()->ridges.size() == 1 && ridge->points->size() < FingerprintContext::minFragmentLength) {
				ridge->detach();
			}
		}
	}
	removeDots();
}
void FingerprintSkeleton::removeKnots() {
	for (SkeletonMinutia* minutia : minutiae) {
		if (minutia->ridges.size() == 2) {
			if (minutia->ridges[0]->reversed != minutia->ridges[1]) {
				SkeletonRidge* extended = minutia->ridges[0]->reversed;
				SkeletonRidge* removed = minutia->ridges[1];
				if (extended->points->size() < removed->points->size()) {
					SkeletonRidge* tmp = extended;
					extended = removed;
					removed = tmp;
					extended = extended->reversed;
					removed = removed->reversed;
				}
				extended->points->pop_back();
				extended->reversed->points->pop_front();
				for (Cell point : *removed->points) {
					extended->points->push_back(point);
					extended->reversed->points->push_front(point);
				}
				extended->end(removed->end());
				removed->detach();
			}
		}
	}
	removeDots();
}
void FingerprintSkeleton::disableBranchMinutiae() {
	for (SkeletonMinutia* minutia : minutiae) {
		if (minutia->ridges.size() > 2) {
			minutia->considered = false;
		}
	}
}
void FingerprintSkeleton::addMinutia(SkeletonMinutia* minutia) {
	minutiae.push_back(minutia);
}
void FingerprintSkeleton::removeMinutia(SkeletonMinutia* minutia) {
	minutiae.remove(minutia);
}
BooleanMap FingerprintSkeleton::shadow() {
	BooleanMap shadow = BooleanMap(size.x, size.y);
	for (SkeletonMinutia* minutia : minutiae) {
		shadow.set(minutia->position, true);
		for (SkeletonRidge* ridge : minutia->ridges) {
			if (ridge->start()->position.y <= ridge->end()->position.y) {
				for (Cell point : *ridge->points) {
					shadow.set(point, true);
				}
			}
		}
	}
	return shadow;
}
