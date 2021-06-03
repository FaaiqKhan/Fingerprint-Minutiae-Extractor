#include "VotingParameters.hpp"
VotingParameters::VotingParameters() {
	radius = 1;
	majority = 0.5;
	borderDist = 0;
}
VotingParameters::VotingParameters(int localRadius, double localMajority, int localBorderDist) {
	radius = localRadius;
	majority = localMajority;
	borderDist = localBorderDist;
}


