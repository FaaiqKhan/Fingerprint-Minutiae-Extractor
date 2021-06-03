#include "Doubles.hpp"
double Doubles::sq(double value) {
	return value * value;
}
double Doubles::interpolate(double start, double end, double position) {
	return start + position * (end - start);
}
double Doubles::interpolate(double topLeft, double topRight, double bottomLeft, double bottomRight, double x, double y) {
	double left = interpolate(bottomLeft, topLeft, y);
	double right = interpolate(bottomRight, topRight, y);
	return interpolate(left, right, x);// (left + (x * (right - left)));
}
double Doubles::interpolateExponential(double start, double end, double position) {
	return (std::pow((end / start), position) * start);
}
