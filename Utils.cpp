#include "Utils.hpp"
bool Utils::equals(Cell next, Cell previous) {
	if (next.x == previous.x && next.y == previous.y) {
		return true;
	} else {
		return false;
	}
}
bool Utils::equals(SkeletonRidge* obj1, SkeletonRidge* obj2) {
	if (obj1 != nullptr && obj2 != nullptr) {
		int listSize = obj1->points->size();
		if (listSize == obj2->points->size()) {
			int matchingCount = 0;
			std::list<Cell>::iterator itr1 = obj1->points->begin();
			std::list<Cell>::iterator itr2 = obj2->points->begin();
			for (int i = 1; i < listSize; ++i) {
				if (itr1->x == itr2->x && itr1->y == itr2->y) {
					matchingCount++;
				} else {
					return false;
				}
			}
			if (matchingCount == listSize) {
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
}
bool Utils::equals(std::list<Cell>* obj1, std::list<Cell>* obj2) {
	if (obj1 != nullptr && obj2 != nullptr) {
		int listSize = obj1->size();
		if (listSize == obj2->size()) {
			int matchingCount = 0;
			std::list<Cell>::iterator itr1 = obj1->begin();
			std::list<Cell>::iterator itr2 = obj2->begin();
			for (int i = 1; i < listSize; ++i) {
				if (itr1->x == itr2->x && itr1->y == itr2->y) {
					matchingCount++;
					std::advance(itr1, i);
					std::advance(itr2, i);
				} else {
					return false;
				}
			}
			if (matchingCount == listSize) {
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
}
bool Utils::equals(SkeletonMinutia* obj1, SkeletonMinutia* obj2) {
	if (obj1 != nullptr && obj2 != nullptr) {
		if (obj1->considered == obj2->considered) {
			if (obj1->position.x == obj2->position.x && obj1->position.y == obj2->position.y) {
				if (obj1->ridges.size() == obj2->ridges.size()) {
					if (Utils::equals(obj1->ridges, obj2->ridges)) {
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
	} else {
		return false;
	}
}
bool Utils::equals(std::vector<SkeletonRidge* > obj1, std::vector<SkeletonRidge* > obj2) {
	int list1Size = obj1.size();
	if (list1Size == obj2.size()) {
		int matchingCount = 0;
		for (int i = 0; i < list1Size; ++i) {
			if (equals(obj1[i], obj2[i])) {
				matchingCount++;
			} else {
				return false;
			}
		}
		if (matchingCount == list1Size) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
bool Utils::equals(std::list<SkeletonRidge>* obj1, std::list<SkeletonRidge>* obj2) {
	int list1Size = obj1->size();
	if (list1Size == obj2->size()) {
		int matchingCount = 0;
		std::list<SkeletonRidge>::iterator itr1 = obj1->begin();
		std::list<SkeletonRidge>::iterator itr2 = obj2->begin();
		for (int i = 1; i < list1Size; ++i) {
			if (itr1->points->size() == itr2->points->size()) {
				matchingCount++;
				std::advance(itr1, i);
				std::advance(itr2, i);
			} else {
				return false;
			}
		}
		if (matchingCount == list1Size) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
bool Utils::equals(std::list<SkeletonRidge* > obj1, std::list<SkeletonRidge* > obj2) {
	int list1Size = obj1.size();
	if (list1Size == obj2.size()) {
		int matchingCount = 0;
		std::list<SkeletonRidge* >::iterator itr1 = obj1.begin();
		std::list<SkeletonRidge* >::iterator itr2 = obj2.begin();
		for (int i = 1; i < list1Size; ++i) {
			SkeletonRidge* ti = *itr1;
			SkeletonRidge* tt = *itr2;
			if (ti->points->size() == tt->points->size()) {
				matchingCount++;
				std::advance(itr1, i);
				std::advance(itr2, i);
			} else {
				return false;
			}
			delete ti;
			delete tt;
		}
		if (matchingCount == list1Size) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
bool Utils::contains(std::set<FingerprintMinutia* > removed, FingerprintMinutia* minutia) {
	std::set<FingerprintMinutia* >::iterator itr;
	for (itr = removed.begin(); itr != removed.end(); ++itr) {
		FingerprintMinutia* temp = *itr;
		if (temp->position.x == minutia->position.x && temp->position.y == minutia->position.y) {
			if (temp->direction == minutia->direction) {
				if (temp->type == minutia->type) {
					return true;
				}
			}
		}
	}
	return false;
}
bool Utils::containsKey(std::map<Cell, std::list<Cell>* > links, Cell neighbor) {
	std::map<Cell, std::list<Cell>* >::iterator itr;
	for (itr = links.begin(); itr != links.end(); itr++) {
		if (itr->first.x == neighbor.x && itr->first.y == neighbor.y) {
			return true;
		}
	}
	return false;
}
bool Utils::containsKey(std::map<Cell, std::list<Cell>* > linking, Cell neighborPos, std::list<Cell>* neighborLinks) {
	std::map<Cell, std::list<Cell>* >::iterator itr;
	for (itr = linking.begin(); itr != linking.end(); itr++) {
		if (itr->first.x == neighborPos.x && itr->first.y == neighborPos.y) {
			neighborLinks = itr->second;
			return true;
		}
	}
	return false;
}
bool Utils::containsKey(std::map<Cell, SkeletonRidge* > map, Cell skeletonRidgeKey) {
	std::map<Cell, SkeletonRidge* >::iterator itr;
	for (itr = map.begin(); itr != map.end(); ++itr) {
		if (itr->first.x == skeletonRidgeKey.x && itr->first.y == skeletonRidgeKey.y) {
			return true;
		}
	}
	return false;
}
bool Utils::containsKey(std::map<Cell, SkeletonMinutia* > map, Cell skeletonMinutiaKey) {
	std::map<Cell, SkeletonMinutia* >::iterator itr;
	for (itr = map.begin(); itr != map.end(); itr++) {
		if (itr->first.x == skeletonMinutiaKey.x && itr->first.y == skeletonMinutiaKey.y) {
			return true;
		}
	}
	return false;
}
bool Utils::isFalseEnding(BooleanMap binary, Cell ending) {
	for (Cell relativeNeighbor : Cell::cornerNeighbors) {
		Cell neighbor = ending.plus(relativeNeighbor);
		if (binary.get(neighbor)) {
			int count = 0;
			for (Cell relative2 : Cell::cornerNeighbors) {
				if (binary.get(neighbor.plus(relative2), false)) {
					++count;
				}
			}
			return count > 2;
		}
	}
	return false;
}
bool Utils::lessThen(SkeletonMinutia* obj1, SkeletonMinutia* obj2) {
	if (obj1->ridges.size() < obj2->ridges.size()) {
		return true;
	} else {
		return false;
	}
}
void Utils::intToBytes(std::vector<unsigned char>* data, int value) {
	data->push_back(0xff & (value >> 24));
	data->push_back(0xff & (value >> 16));
	data->push_back(0xff & (value >> 8));
	data->push_back(0xff & value);
}
void Utils::shortToBytes(std::vector<unsigned char>* data, short value) {
	data->push_back(0xff & (value >> 8));
	data->push_back(0xff & value);
}
int Utils::readUnsignedShort(int* index, std::vector<unsigned char> data) {
	int val = ((data[(*index)] << 8) | (data[(*index) + 1] & 0xff));
	*index = *index + 2;
	return val;
}
int Utils::readInt(int* index, std::vector<unsigned char> data) {
	int val = (((data[(*index)] & 0xff) << 24) | ((data[(*index) + 1] & 0xff) << 16) | ((data[(*index) + 2] & 0xff) << 8) | ((data[(*index) + 3] & 0xff)));
	*index = *index + 4;
	return val;
}
int Utils::readShort(int* index, std::vector<unsigned char> data) {
	int val = (((data[(*index)] & 0xff) << 8) | (data[(*index) + 1] & 0xff));
	*index = *index + 2;
	return val;
}
int Utils::readUnsignedByte(int* index, std::vector<unsigned char> data) {
	int val = (int)(data[(*index)]);
	*index = *index + 1;
	return val;
}
std::list<Cell>* Utils::get(std::map<Cell, std::list<Cell>* > map, Cell key) {
	std::map<Cell, std::list<Cell>* >::iterator itr;
	for (itr = map.begin(); itr != map.end(); itr++) {
		Cell cell = itr->first;
		if (cell.x == key.x && cell.y == key.y) {
			return itr->second;
		}
	}
	return nullptr;
}
