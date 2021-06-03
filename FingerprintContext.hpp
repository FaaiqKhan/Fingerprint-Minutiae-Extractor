#ifndef FINGERPRINTCONTEXT_H
#define FINGERPRINTCONTEXT_H
#include "VotingParameters.hpp"
#include "OrientedLineParams.hpp"
class FingerprintContext {
	public:
		static int blockSize;
		static double dpiTolerance;
		static int histogramDepth;
		static double clippedContrast;
		static double minAbsoluteContrast;
		static double minRelativeContrast;
		static int relativeContrastSample;
		static double relativeContrastPercentile;
		static VotingParameters contrastVote;
		static VotingParameters blockErrorsVote;
		static VotingParameters maskVote;
		static VotingParameters binarizedVote;
		static double maxEqualizationScaling;
		static double minEqualizationScaling;
		static double minOrientationRadius;
		static double maxOrientationRadius;
		static int orientationSplit;
		static int orientationsChecked;
		static int orientationSmoothingRadius;
		static OrientedLineParams parallelSmoothining;
		static OrientedLineParams orthogonalSmoothing;
		static int innerMaskBorderDistance;
		static double maskDisplacement;
		static double maxAngleError;
		static double maxGapAngle;
		static int minutiaCloudRadius;
		static int maxCloudSize;
		static int maxMinutiae;
		static int sortByNeighbor;
		static int edgeTableRange;
		static int edgeTableNeighbors;
		static int thinningIterations;
		static int maxPoreArm;
		static int shortestJoinedEnding;
		static int maxRuptureSize;
		static int maxGapSize;
		static int gapAngleOffset;
		static int minFragmentLength;
		static int toleratedGapOverlap;
		static int minTailLength;
		static int ridgeDirectionSkip;
		static int ridgeDirectionSample;
};
#endif
