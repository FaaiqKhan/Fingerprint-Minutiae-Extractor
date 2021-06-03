#ifndef FINGERPRINTMINUTIA_H
#define FINGERPRINTMINUTIA_H
#include "Cell.hpp"
#include "MinutiaType.cpp"
class FingerprintMinutia {
	public:
		Cell position;
		double direction;
		MinutiaType type;
		FingerprintMinutia(Cell pOsition, double dIrection, MinutiaType tYpe);
};
#endif
