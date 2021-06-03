#include "Range.hpp"
Range::Range(int localStart, int localEnd) {
	start = localStart;
	end = localEnd;
}
int Range::lenght() {
	return end - start;
}
