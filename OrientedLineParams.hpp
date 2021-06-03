#ifndef ORIENTEDLINEPARAMS_H
#define ORIENTEDLINEPARAMS_H
class OrientedLineParams {
	public:
		int resolution = 5;
		int radius = 10;
		double step = 1.5;
		OrientedLineParams();
		OrientedLineParams(int res, int rad, double stp);
		void setResolution(int localResolution);
		void setRadius(int localRadius);
		void setStep(double localStep);
};
#endif
