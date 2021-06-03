#include <list>
#include <vector>
#include <iostream>
#include "Angle.hpp"
#include "Range.hpp"
#include "Random.hpp"
#include "Doubles.hpp"
#include "PointMap.hpp"
#include "BlockMap.hpp"
#include "DoubleMap.hpp"
#include "Histogram.hpp"
#include "BooleanMap.hpp"
#include "MinutiaType.cpp"
#include "VotingParameters.hpp"
#include "FingerprintContext.hpp"
#include "FingerprintMinutia.hpp"
#include "OrientedLineParams.hpp"
#include "FingerprintSkeleton.hpp"
struct ConsideredOrientation {
	Cell offset;
	Point orientation;
};
class FingerprintTemplate {
	private:
		std::vector<FingerprintMinutia* > minutiae;
		DoubleMap readImage(std::vector<signed int> pixels, int width, int height); // Function that take primitive datatype.
		BlockMap blockMap(int width, int height, int maxBlockSize);
		Histogram cHistogram(BlockMap blocks, DoubleMap image);
		Histogram cSmoothHistogram(BlockMap blocks, Histogram input);
		BooleanMap cMask(BlockMap blocks, Histogram histogram);
		DoubleMap equalize(BlockMap blocks, DoubleMap image, Histogram histogram, BooleanMap blockMask);
		DoubleMap orientationMap(DoubleMap image, BooleanMap mask, BlockMap blocks);
		DoubleMap smoothRidges(DoubleMap input, DoubleMap orientation, BooleanMap mask, BlockMap blocks, double angle, std::vector<std::list<Cell> > lines);
		BooleanMap binarize(DoubleMap input, DoubleMap baseLine, BooleanMap mask, BlockMap blocks);
		BooleanMap cleanupBinarized(BooleanMap binary);
		BooleanMap fillBlocks(BooleanMap mask, BlockMap blocks);
		BooleanMap invert(BooleanMap binary, BooleanMap mask);
		BooleanMap cInnerMask(BooleanMap outer);
		void collectMinutiae(FingerprintSkeleton ridges, MinutiaType minutiaType);
    	void maskMinutiae(BooleanMap innerMask);
    	void removeMinutiaClouds();
		void limitTemplateSize();
		void shuffleMinutiae();
		DoubleMap clipContrast(BlockMap blocks, Histogram histogram);
		BooleanMap filterAbsoluteContrast(DoubleMap contrast);
		double getAverage(std::vector<double> sortedContrast, int limit);
		BooleanMap filterRelativeContrast(DoubleMap contrast, BlockMap blocks);
		BooleanMap vote(BooleanMap input, VotingParameters args);
		BooleanMap filterBlockErrors(BooleanMap input);
		bool anyMatch(std::vector<ConsideredOrientation* > orientations, int limit, Cell offset);
		static Range CalculateMaskRange(BooleanMap mask, int y);
		PointMap pixelwiseOrientation(DoubleMap input, BooleanMap mask, BlockMap blocks);
		PointMap blockOrientations(PointMap orientation, BlockMap blocks, BooleanMap mask);
		PointMap smoothOrientation(PointMap orientation, BooleanMap mask);
		DoubleMap orientationAngles(PointMap vectors, BooleanMap mask);
		std::vector<std::list<Cell> > orientedLines(OrientedLineParams args);
		BooleanMap shrinkMask(BooleanMap mask, int amount);
		BooleanMap removeCrosses(BooleanMap input);
		std::vector<FingerprintMinutia* > filterMaskMinutiae(BooleanMap mask);
		std::set<FingerprintMinutia* > filterRemoveMinutiaClouds();
		std::vector<FingerprintMinutia* > filterRemoveMinutiaClouds(std::set<FingerprintMinutia* > removed);
		std::vector<FingerprintMinutia* > sortAndLimit_LimitTemplateSize();
		std::vector<std::vector<ConsideredOrientation* > > planOrientations();
	public:
		FingerprintTemplate(std::vector<signed int> pixels, int width, int height);
		std::vector<FingerprintMinutia* > getMinutiae();
		static std::vector<unsigned char> streamIso(std::vector<FingerprintMinutia* > fingerprintMinutiae, int bitmapWidth, int bitmapHeight);
		static std::vector<FingerprintMinutia* > parseIso(std::vector<unsigned char> minutia);
};
