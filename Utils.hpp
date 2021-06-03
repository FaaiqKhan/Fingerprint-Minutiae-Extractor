#ifndef UTILS_H
#define UTILS_H
#include <set>
#include <map>
#include <list>
#include <vector>
#include "BooleanMap.hpp"
#include "SkeletonRidge.hpp"
#include "SkeletonMinutia.hpp"
#include "FingerprintMinutia.hpp"
class Utils {
	public:
		static bool equals(Cell next, Cell previous);
		static bool equals(SkeletonRidge* obj1, SkeletonRidge* obj2);
		static bool equals(std::list<Cell>* obj1, std::list<Cell>* obj2);
		static bool equals(SkeletonMinutia* obj1, SkeletonMinutia* obj2);
		static bool equals(std::vector<SkeletonRidge* > obj1, std::vector<SkeletonRidge* > obj2);
		static bool equals(std::list<SkeletonRidge>* obj1, std::list<SkeletonRidge>* obj2);
		static bool equals(std::list<SkeletonRidge* > obj1, std::list<SkeletonRidge* > obj2);
		static bool contains(std::set<FingerprintMinutia* > removed, FingerprintMinutia* minutia);
		static bool containsKey(std::map<Cell, std::list<Cell>* > links, Cell neighbors);	
		static bool containsKey(std::map<Cell, SkeletonRidge* > map, Cell skeletonRidgeKey);
		static bool containsKey(std::map<Cell, SkeletonMinutia* > map, Cell skeletonMinutiaKey);
		static bool containsKey(std::map<Cell, std::list<Cell>* > linking, Cell neighborPos, std::list<Cell>* neighborLinks);
		static bool isFalseEnding(BooleanMap binary, Cell ending);
		static bool lessThen(SkeletonMinutia* obj1, SkeletonMinutia* obj2);
		static void intToBytes(std::vector<unsigned char>* data, int value);
		static void shortToBytes(std::vector<unsigned char>* data, short value);
		static int readUnsignedShort(int* index, std::vector<unsigned char> data);
		static int readInt(int* index, std::vector<unsigned char> data);
		static int readShort(int* index, std::vector<unsigned char> data);
		static int readUnsignedByte(int* index, std::vector<unsigned char> data);
		static std::list<Cell>* get(std::map<Cell, std::list<Cell>* > map, Cell key);
};

#endif
