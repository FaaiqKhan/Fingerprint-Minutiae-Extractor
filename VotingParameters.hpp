#ifndef VOTINGPARAMETERS_H
#define VOTINGPARAMETERS_H
class VotingParameters {
	public:
		int radius;
		double majority;
		int borderDist;
		VotingParameters();
		VotingParameters(int localRadius,  double localMajority, int borderDist);
};
#endif
