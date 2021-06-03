#include <time.h>
class myRandom {
	private:
		long long temp;
		long long temp2;
		long long* seed;
	public:
		myRandom();
		void setSeed(long long localSeed);
		int next(int bits);
		double nextDouble();
};
