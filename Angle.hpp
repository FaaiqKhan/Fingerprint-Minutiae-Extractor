#ifndef Angle_H
#define Angle_H
#include "Point.hpp"
#include "cmath"
class Angle {
	public:
		static double PI;
		static double PI2;
		static double invPI2;
		static Point toVector(double angle);
		static double atan(Point vector);
		static double atan(Cell vector);
		static double atan(Cell center, Cell point);
		static double toOrientation(double angel);
		static double add(double start, double delta);
		static double bucketCenter(int bucket, int resolution);
		static int quantize(double angle, int resolution);
		static double opposite(double angle);
		static double distance(double first, double second);
		static double difference(double first, double second);
		static double complementary(double angle);
};
#endif
