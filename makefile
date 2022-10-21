#!/bin/sh
LIBS="-L"/usr/x86_64-w64-mingw32/lib" -static-libgcc "/usr/x86_64-w64-mingw32/lib/libgdi32.a" -lpthread -lm"

targets: prerequisites
	echo "processing..."
	g++ -c Angle.cpp -o Angle.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Angle.o created."
	g++ -c Block.cpp -o Block.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Block.o created."
	g++ -c BlockGrid.cpp -o BlockGrid.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "BlockGrid.o created."
	g++ -c BlockMap.cpp -o BlockMap.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "BlockMap.o created."
	g++ -c BooleanMap.cpp -o BooleanMap.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "BooleanMap.o created."
	g++ -c Cell.cpp -o Cell.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Cell.o created."
	g++ -c CellGrid.cpp -o CellGrid.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "CellGrid.o created."
	g++ -c DoubleMap.cpp -o DoubleMap.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "DoubleMap.o created."
	g++ -c Doubles.cpp -o Doubles.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Doubles.o created."
	g++ -c FingerprintContext.cpp -o FingerprintContext.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "FingerprintContext.o created."
	g++ -c FingerprintMinutia.cpp -o FingerprintMinutia.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "FingerprintMinutia.o created."
	g++ -c FingerprintSkeleton.cpp -o FingerprintSkeleton.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "FingerprintSkeleton.o created."
	g++ -c FingerprintTemplate.cpp -o FingerprintTemplate.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "FingerprintTemplate.o created."
	g++ -c Gap.cpp -o Gap.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Gap.o created."
	g++ -c Histogram.cpp -o Histogram.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Histogram.o created."
	g++ -c Integers.cpp -o Integers.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Integers.o created."
	g++ -c MinutiaType.cpp -o MinutiaType.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "MinutiaType.o created."
	g++ -c OrientedLineParams.cpp -o OrientedLineParams.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "OrientedLineParams.o created."
	g++ -c Point.cpp -o Point.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Point.o created."
	g++ -c PointMap.cpp -o PointMap.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "PointMap.o created."
	g++ -c Random.cpp -o Random.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Random.o created."
	g++ -c Range.cpp -o Range.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Range.o created."
	g++ -c SkeletonMinutia.cpp -o SkeletonMinutia.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "SkeletonMinutia.o created."
	g++ -c SkeletonRidge.cpp -o SkeletonRidge.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "SkeletonRidge.o created."
	g++ -c Utils.cpp -o Utils.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "Utils.o created."
	g++ -c VotingParameters.cpp -o VotingParameters.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "VotingParameters.o created."
	g++ -c main.cpp -o main.o -std=c++11 -lgdi32 -O2 -lpthread -lm
	echo "main.o created."
	g++ main.o Cell.o Integers.o CellGrid.o Block.o BlockGrid.o DoubleMap.o BlockMap.o Histogram.o BooleanMap.o VotingParameters.o Point.o PointMap.o Range.o OrientedLineParams.o SkeletonRidge.o SkeletonMinutia.o FingerprintSkeleton.o Angle.o FingerprintContext.o Gap.o MinutiaType.o FingerprintMinutia.o Utils.o Random.o FingerprintTemplate.o Doubles.o -o fingerprintTemplate.exe
	echo "Completed"
# To run the exe file
	chmod +x fingerprinttemplate.exe
	./fingerprinttemplate.exe

prerequisites:
	echo "deleting old files"
# If getting error of file not exist then comment out the rm command
	rm *.o fingerprinttemplate.exe