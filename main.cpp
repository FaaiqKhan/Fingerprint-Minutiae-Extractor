#define cimg_display 0

#include <iostream>
#include "CImg/CImg.h"
#include "FingerprintTemplate.hpp"

using namespace cimg_library;

int main() {
	// std::cout<<"Program started..."<<std::endl;
	CImg<unsigned char> src("dummy.bmp");
	// std::cout<<"Image loaded into memory."<<std::endl;
	int width = src.width();
	// std::cout<<"Image width: "<<width<<std::endl;
	int height = src.height();
	// std::cout<<"Image height: "<<height<<std::endl;
	std::vector<signed int> pixels(width * height);
	// std::cout<<"Vector initialized."<<std::endl;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			int R = src(c, r, 0, 0);
			int G = src(c, r, 0, 1);
			int B = src(c, r, 0, 2);
			pixels[r * width + c] = (B << 16) + (G << 8) + R;
		}
	}
	// std::cout<<"Pixels variable loaded with data and going for process."<<std::endl;
	FingerprintTemplate fingerprintTemplate = FingerprintTemplate(pixels, width, height);
	
	std::cout<<"Data extracted ready to show data on screen."<<std::endl;
	std::vector<FingerprintMinutia* > fingerprintMinutiae = fingerprintTemplate.getMinutiae();
	std::cout<<"size of minutiae = "<<fingerprintMinutiae.size()<<std::endl;
	for (FingerprintMinutia* fm : fingerprintMinutiae)
		std::cout<<(fm->type==0?"ENDING":"BIFURCATION")<<"["<<fm->position.x<<","<<fm->position.y<<"]@"<<fm->direction<<std::endl;
	/*std::vector<unsigned char> iso = FingerprintTemplate::streamIso(fingerprintMinutiae, width, height);
	std::cout<<"size of iso: "<<iso.size()<<std::endl;
	std::vector<FingerprintMinutia* > parsedData = FingerprintTemplate::parseIso(iso);
	for (FingerprintMinutia* fm : parsedData) {
		std::cout<<(fm->type==0?"ENDING":"BIFURCATION")<<"["<<fm->position.x<<","<<fm->position.y<<"]@"<<fm->direction<<std::endl;
	}*/
	return 0;
}
