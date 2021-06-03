#ifndef DOUBLES_H
#define DOUBLES_H
#include "cmath"
class Doubles {
	public:
		static double sq(double value);
		static double interpolate(double start, double end, double position);
		static double interpolate(double topLeft, double topRight, double bottomLeft, double bottomRight, double x, double y);
		static double interpolateExponential(double start, double end, double position);
};
#endif
