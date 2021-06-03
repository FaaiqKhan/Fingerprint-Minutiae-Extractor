#include "Angle.hpp"
double Angle::PI = 3.141592653589793;
double Angle::PI2 = 2 * PI;
double Angle::invPI2 = 1.0 / PI2;
Point Angle::toVector(double angle) {
	return Point(cos(angle), sin(angle));
}
double Angle::atan(Point vector) {
	double angle = atan2(vector.y, vector.x);
	return angle >= 0 ? angle : angle + PI2;
}
double Angle::atan(Cell vector) {
	return atan(vector.toPoint());
}
double Angle::atan(Cell center, Cell point) {
	return atan(point.minus(center));
}
double Angle::toOrientation(double angle) {
	return angle < Angle::PI ? 2 * angle : 2 * (angle - Angle::PI);
}
double Angle::add(double start, double delta) {
	double angle = start + delta;
	return angle < Angle::PI2 ? angle : angle - Angle::PI2;
}
double Angle::bucketCenter(int bucket, int resolution) {
	return Angle::PI2 * (2 * bucket + 1) / (2 * resolution);
}
int Angle::quantize(double angle, int resolution) {
	int result = (int)(angle * Angle::invPI2 * resolution);
	if (result < 0) {
		return 0;
	} else if (result >= resolution) {
		return resolution - 1;
	} else {
		return result;
	}
}
double Angle::opposite(double angle) {
	return angle < Angle::PI ? angle + Angle::PI : angle - Angle::PI;
}
double Angle::distance(double first, double second) {
	double delta = std::abs(first - second);
	return delta <= Angle::PI ? delta : Angle::PI2 - delta;
}
double Angle::difference(double first, double second) {
	double angle = first - second;
	return angle >= 0 ? angle : angle + Angle::PI2;
}
double Angle::complementary(double angle) {
	double complement = Angle::PI2 - angle;
	return complement < Angle::PI2 ? complement : complement - Angle::PI2;
}
