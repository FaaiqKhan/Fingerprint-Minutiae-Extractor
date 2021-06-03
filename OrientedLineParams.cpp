#include "OrientedLineParams.hpp"
OrientedLineParams::OrientedLineParams() {
}
OrientedLineParams::OrientedLineParams(int res, int rad, double stp) {
	resolution = res;
	radius = rad;
	step = stp;
}
void OrientedLineParams::setResolution(int localResolution) {
	resolution = localResolution;
}
void OrientedLineParams::setRadius(int localRadius) {
	radius = localRadius;
}
void OrientedLineParams::setStep(double localStep) {
	step = localStep;
}
