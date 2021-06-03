#include "FingerprintTemplate.hpp"
FingerprintTemplate::FingerprintTemplate(std::vector<signed int> pixels, int width, int height) {
	// std::cout<<"In fingerprint template class."<<std::endl;
	DoubleMap raw = readImage(pixels, width, height); // Checked.
	// std::cout<<"ReadImage"<<std::endl;
	BlockMap blocks = BlockMap(raw.width, raw.height, FingerprintContext::blockSize); // Checked.
	// std::cout<<"BlockMap"<<std::endl;
	Histogram histogram = cHistogram(blocks, raw); // Checked.
	// std::cout<<"cHistogram"<<std::endl;
	Histogram smoothHistogram = cSmoothHistogram(blocks, histogram); // Checked.
	// std::cout<<"smoothHistogram"<<std::endl;
	BooleanMap mask = cMask(blocks, histogram); // Checked.
	// std::cout<<"cMask"<<std::endl;
	DoubleMap equalized = equalize(blocks, raw, smoothHistogram, mask); // Checked.
	// std::cout<<"equalize"<<std::endl;
	DoubleMap orientation = orientationMap(equalized, mask, blocks); // Checked.
	// std::cout<<"orientationMap"<<std::endl;
	DoubleMap smoothed = smoothRidges(equalized, orientation, mask, blocks, 0, orientedLines(FingerprintContext::parallelSmoothining)); // Checked.
	// std::cout<<"smoothRidges"<<std::endl;
	DoubleMap orthogonal = smoothRidges(smoothed, orientation, mask, blocks, Angle::PI, orientedLines(FingerprintContext::orthogonalSmoothing)); // Checked.
	// std::cout<<"smoothRidges"<<std::endl;
	BooleanMap binary = binarize(smoothed, orthogonal, mask, blocks); // Checked.
	// std::cout<<"binarize"<<std::endl;
	BooleanMap updatedBinary = cleanupBinarized(binary); // Checked.
	// std::cout<<"cleanupBinarized"<<std::endl;
	BooleanMap pixelMask = fillBlocks(mask, blocks); // Checked.
	// std::cout<<"fillBlocks"<<std::endl;
	BooleanMap inverted = invert(updatedBinary, pixelMask); // Checked.
	// std::cout<<"invert"<<std::endl;
	BooleanMap innerMask = cInnerMask(pixelMask); // Checked.
	// std::cout<<"cInnerMask"<<std::endl;
	FingerprintSkeleton ridges = FingerprintSkeleton(updatedBinary);
	// std::cout<<"FingerprintSkeleton ridges"<<std::endl;
	FingerprintSkeleton valleys = FingerprintSkeleton(inverted);
	// std::cout<<"FingerprintSkeleton valleys"<<std::endl;
	collectMinutiae(ridges, MinutiaType::ENDING);
	// std::cout<<"collectMinutiae"<<std::endl;
	collectMinutiae(valleys, MinutiaType::BIFURCATION);
	// std::cout<<"collectMinutiae"<<std::endl;
	maskMinutiae(innerMask);
	// std::cout<<"maskMinutiae"<<std::endl;
	removeMinutiaClouds();
	// std::cout<<"removeMinutiaClouds"<<std::endl;
	limitTemplateSize();
	// std::cout<<"limitTemplateSize"<<std::endl;
	shuffleMinutiae();
	// std::cout<<"shuffleMinutiae"<<std::endl;
}
std::vector<FingerprintMinutia* > FingerprintTemplate::getMinutiae() {
	return minutiae;
}
DoubleMap FingerprintTemplate::readImage(std::vector<signed int> pixels, int width, int height) {
	DoubleMap map = DoubleMap(width, height);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int pixel = pixels[y * width + x];
			int color = (pixel & 0xff) + ((pixel >> 8) & 0xff) + ((pixel >> 16) & 0xff);
			double val = 1 - color * (1.0 / (3.0 * 255.0));
			map.set(x, height - y - 1, 1 - color * (1.0 / (3.0 * 255.0)));
		}
	}
	return map;
}
Histogram FingerprintTemplate::cHistogram(BlockMap blocks, DoubleMap image) {
	Histogram localHistogram = Histogram(blocks.blockCount.x, blocks.blockCount.y, FingerprintContext::histogramDepth);
	for (int bY = 0; bY < blocks.blockCount.y; ++bY) {
		for (int bX = 0; bX < blocks.blockCount.x; ++bX) {
			Cell block = Cell(bX, bY);
			Block area = blocks.blockAreas.get(block);
			for (int y = area.bottom(); y < area.top(); ++y) {
				for (int x = area.left(); x < area.right(); ++x) {
					int depth = (int)(image.get(x, y) * localHistogram.depth);
					localHistogram.increment(block, localHistogram.constrain(depth));
				}
			}
		}
	}
	return localHistogram;
}
Histogram FingerprintTemplate::cSmoothHistogram(BlockMap blocks, Histogram input) {
	Cell blocksAround[4] = { Cell(0, 0), Cell(-1, 0), Cell(0, -1), Cell(-1, -1) };
	Histogram output = Histogram(blocks.cornerCount.x, blocks.cornerCount.y, input.depth);
	for (int bY = 0; bY < blocks.cornerCount.y; ++bY) {
		for (int bX = 0; bX < blocks.cornerCount.x; ++bX) {
			Cell corner = Cell(bX, bY);
			for (int index = 0; index < 4; index++) {
				Cell block = corner.plus(blocksAround[index]);
				if (blocks.blockCount.contains(block)) {
					for (int i = 0; i < input.depth; ++i) {
						output.add(corner, i, input.get(block, i));
					}
				}
			}
		}
	}
	return output;
}
DoubleMap FingerprintTemplate::clipContrast(BlockMap blocks, Histogram histogram) {
	DoubleMap result = DoubleMap(blocks.blockCount.x, blocks.blockCount.y);
	for (int bY = 0; bY < blocks.blockCount.y; ++bY) {
		for (int bX = 0; bX < blocks.blockCount.x; ++bX) {
			Cell block = Cell(bX, bY);
			int volume = histogram.sum(block);
			int clipLimit = (int)round(volume * 0.08);
			int accumulator = 0;
			int lowerBound = histogram.depth - 1;
			for (int i = 0; i < histogram.depth; ++i) {
				accumulator += histogram.get(block, i);
				if (accumulator > clipLimit) {
					lowerBound = i;
					break;
				}
			}
			accumulator = 0;
            int upperBound = 0;
            for (int i = histogram.depth - 1; i >= 0; --i) {
            	accumulator += histogram.get(block, i);
            	if (accumulator > clipLimit) {
            		upperBound = i;
            		break;
				}
			}
			result.set(block, (upperBound - lowerBound) * (1.0 / (histogram.depth - 1)));
		}
	}
	return result;
}
BooleanMap FingerprintTemplate::filterAbsoluteContrast(DoubleMap contrast) {
	BooleanMap result = BooleanMap(contrast.size().x, contrast.size().y);
	for (int cY = 0; cY < contrast.size().y; ++cY) {
		for (int cX = 0; cX < contrast.size().x; ++cX) {
			Cell block = Cell(cX, cY);
			if (contrast.get(block) < (17 / 255.0)) {
				result.set(block, true);
			}
		}
	}
	return result;
}
double FingerprintTemplate::getAverage(std::vector<double> sortedContrast, int limit) {
	double total = 0;
	for (int i = 0; i < limit; i++) {
		total += sortedContrast[i];
	}
	return (total / limit);
}
BooleanMap FingerprintTemplate::filterRelativeContrast(DoubleMap contrast, BlockMap blocks) {
	std::vector<double> sortedContrast;
	for (int cY = 0; cY < contrast.size().y; ++cY) {
		for (int cX = 0; cX < contrast.size().x; ++cX) {
			Cell block = Cell(cX, cY);
			sortedContrast.push_back(contrast.get(block));
		}
	}
	std::sort(sortedContrast.begin(), sortedContrast.end(), std::greater<double>());
	int pixelsPerBlock = blocks.pixelCount.area() / blocks.blockCount.area();
	int sampleCount = fmin(sortedContrast.size(), (FingerprintContext::relativeContrastSample / pixelsPerBlock));
	int consideredBlocks = fmax((int)round(sampleCount * FingerprintContext::relativeContrastPercentile), 1);
	double averageContrast = getAverage(sortedContrast, consideredBlocks);
	double limit = averageContrast * FingerprintContext::minRelativeContrast;
	BooleanMap result = BooleanMap(blocks.blockCount.x, blocks.blockCount.y);
	for (int cY = 0; cY < blocks.blockCount.y; ++cY) {
		for (int cX = 0; cX < blocks.blockCount.x; ++cX) {
			Cell block = Cell(cX, cY);
			if (contrast.get(block) < limit) {
				result.set(block, true);
			}
		}
	}
	return result;
}
BooleanMap FingerprintTemplate::vote(BooleanMap input, VotingParameters args) {
	Cell size = input.size();
	Block rect = Block(args.borderDist, args.borderDist, size.x - 2 * args.borderDist, size.y - 2 * args.borderDist);
	BooleanMap output = BooleanMap(size.x, size.y);
	for (int cY = 0; cY < rect.height; ++cY) {
		for (int cX = 0; cX < rect.width; ++cX) {
			Cell center = Cell(rect.x + cX, rect.y + cY);
			Block neighborhood = Block::around(center, args.radius).interset(Block(0, 0, size.x, size.y));
			int ones = 0;
			for (int ny = neighborhood.bottom(); ny < neighborhood.top(); ++ny) {
				for (int nx = neighborhood.left(); nx < neighborhood.right(); ++nx) {
					if (input.get(nx, ny)) {
						++ones;
					}
				}
			}
			double voteWeight = 1.0 / neighborhood.area();
			if (ones * voteWeight >= args.majority) {
				output.set(center, true);
			}
		}
	}
	return output;
}
BooleanMap FingerprintTemplate::filterBlockErrors(BooleanMap input) {
	return vote(input, FingerprintContext::blockErrorsVote);
}
BooleanMap FingerprintTemplate::cMask(BlockMap blocks, Histogram histogram) {
	DoubleMap contrast = clipContrast(blocks, histogram);
	BooleanMap mask = filterAbsoluteContrast(contrast);
	mask.merge(filterRelativeContrast(contrast, blocks));
	mask.merge(vote(mask, FingerprintContext::contrastVote));
	mask.merge(filterBlockErrors(mask));
	mask.invert();
	mask.merge(filterBlockErrors(mask));
	mask.merge(filterBlockErrors(mask));
	mask.merge(vote(mask, FingerprintContext::maskVote));
	return mask;
}
DoubleMap FingerprintTemplate::equalize(BlockMap blocks, DoubleMap image, Histogram histogram, BooleanMap blockMask) {
	const double rangeMin = -1;
	const double rangeMax = 1;
	const double rangeSize = rangeMax - rangeMin;
	const double widthMax = rangeSize / 256 * FingerprintContext::maxEqualizationScaling;
	const double widthMin = rangeSize / 256 * FingerprintContext::minEqualizationScaling;
	std::vector<double> limitedMin;
	std::vector<double> limitedMax;
	std::vector<double> dequantized;
	for (int i = 0; i < histogram.depth; ++i) {
		limitedMin.push_back(fmax(i * widthMin + rangeMin, rangeMax - (histogram.depth - 1 - i) * widthMax));
		limitedMax.push_back(fmin(i * widthMax + rangeMin, rangeMax - (histogram.depth - 1 - i) * widthMin));
		dequantized.push_back((i / (double)(histogram.depth - 1)));
	}
	std::map<Cell*, std::vector<double> > mappings;
	for (int cornerCellY = 0; cornerCellY < blocks.cornerCount.y; ++cornerCellY) {
		for (int cornerCellX = 0; cornerCellX < blocks.cornerCount.x; ++cornerCellX) {
			Cell corner = Cell(cornerCellX, cornerCellY);
			std::vector<double> mapping(histogram.depth);
			if (blockMask.get(corner, false) || blockMask.get(corner.x - 1, corner.y, false) || blockMask.get(corner.x, corner.y - 1, false) || blockMask.get(corner.x - 1, corner.y - 1, false)) {
				double step = rangeSize / histogram.sum(corner);
				double top = rangeMin;
				for (int i = 0; i < histogram.depth; ++i) {
					double band = histogram.get(corner, i) * step;
					double equalized = top + dequantized[i] * band;
					top += band;
					if (equalized < limitedMin[i]) {
						equalized = limitedMin[i];
					}
					if (equalized > limitedMax[i]) {
						equalized = limitedMax[i];
					}
					mapping[i] = equalized;
				}
			}
			mappings.insert(std::make_pair(new Cell(corner.x, corner.y), mapping));
		}
	}
	DoubleMap result = DoubleMap(blocks.pixelCount.x, blocks.pixelCount.y);
		for (int blockPositionY = 0; blockPositionY < blocks.blockCount.y; blockPositionY++) {
			for (int blockPositionX = 0; blockPositionX < blocks.blockCount.x; blockPositionX++) {
				Cell block = Cell(blockPositionX, blockPositionY);
				if (blockMask.get(block)) {
					Block area = blocks.blockAreas.get(block);
					std::vector<double> bottomLeft;
					std::vector<double> bottomRight;
					std::vector<double> topLeft;
					std::vector<double> topRight;
					std::map<Cell*, std::vector<double> >::iterator itr;
					for (itr = mappings.begin(); itr != mappings.end(); itr++) {
						Cell tempBlock = itr->first;
						Cell bottomLeftBlock = Cell(block.x, block.y);
						Cell bottomRightBlock = Cell(block.x + 1, block.y);
						Cell topLeftBlock = Cell(block.x, block.y + 1);
						Cell topRightBlock = Cell(block.x + 1, block.y + 1);
						if (tempBlock.x == bottomLeftBlock.x && tempBlock.y == bottomLeftBlock.y) {
							bottomLeft = itr->second;
						}
						if (tempBlock.x == bottomRightBlock.x && tempBlock.y == bottomRightBlock.y) {
							bottomRight = itr->second;
						}
						if (tempBlock.x == topLeftBlock.x && tempBlock.y == topLeftBlock.y) {
							topLeft = itr->second;
						}
						if (tempBlock.x == topRightBlock.x && tempBlock.y == topRightBlock.y) {
							topRight = itr->second;
						}
					}
					for (int y = area.bottom(); y < area.top(); ++y) {
						for (int x = area.left(); x < area.right(); ++x) {
							int depth = histogram.constrain((int)(image.get(x, y) * histogram.depth));
							double rx = (x - area.x + 0.5) / area.width;
							double ry = (y - area.y + 0.5) / area.height;
							result.set(x, y, Doubles::interpolate(topLeft[depth], topRight[depth], bottomLeft[depth], bottomRight[depth], rx, ry));
						}
					}
				}
			}
		}
	return result;
}
bool FingerprintTemplate::anyMatch(std::vector<ConsideredOrientation* > orientations, int limit, Cell offset)  {
	ConsideredOrientation* orientation;
	for (int i = 0; i < limit; i++) {
		orientation = orientations[i];
		if (orientation->offset.x == offset.x && orientation->offset.y == offset.y) {
			return true;
		}
	}
	return false;
}
std::vector<std::vector<ConsideredOrientation* > > FingerprintTemplate::planOrientations() {
	myRandom myRand = myRandom();
	std::vector< std::vector<ConsideredOrientation* > > splits;
	for (int i = 0; i < FingerprintContext::orientationSplit; ++i) {
		std::vector<ConsideredOrientation* > orientations(FingerprintContext::orientationsChecked);
		for (int j = 0; j < FingerprintContext::orientationsChecked; ++j) {
			ConsideredOrientation* sample = orientations[j] = new ConsideredOrientation();
			do {
				double angle = myRand.nextDouble() * Angle::PI;
				double distance = Doubles::interpolateExponential(FingerprintContext::minOrientationRadius, FingerprintContext::maxOrientationRadius, myRand.nextDouble());
				sample->offset = Angle::toVector(angle).multiply(distance).roundPoint();
			} while ((sample->offset.x == 0 && sample->offset.y == 0) || sample->offset.y < 0 || anyMatch(orientations, j, sample->offset));
		sample->orientation = Angle::toVector(Angle::add(Angle::toOrientation(Angle::atan(sample->offset.toPoint())), Angle::PI));
		}
		splits.push_back(orientations);
	}
	return splits;
}
Range FingerprintTemplate::CalculateMaskRange(BooleanMap mask, int y) {
	int first = -1;
	int last = -1;
	for (int x = 0; x < mask.width; ++x) {
		if (mask.get(x, y)) {
			last = x;
			if (first < 0) {
				first = x;
			}
		}
	}
	if (first >= 0) {
		return Range(first, last + 1);
	} else {
		return Range(0, 0);
	}
}
PointMap FingerprintTemplate::pixelwiseOrientation(DoubleMap input, BooleanMap mask, BlockMap blocks) {
	std::vector<std::vector<ConsideredOrientation* > > neighbors = planOrientations();
	PointMap orientations = PointMap(input.size().x, input.size().y);
	for (int blockY = 0; blockY < blocks.blockCount.y; ++blockY) {
		Range maskRange = FingerprintTemplate::CalculateMaskRange(mask, blockY);
		if (maskRange.lenght() > 0) {
			Range validXRange = Range(blocks.blockAreas.get(maskRange.start, blockY).left(), blocks.blockAreas.get(maskRange.end - 1, blockY).right()); 
			for (int y = blocks.blockAreas.get(0, blockY).bottom(); y < blocks.blockAreas.get(0, blockY).top(); ++y) {
				for (ConsideredOrientation* parosi : neighbors[y % neighbors.size()]) {
					int radius = fmax(abs(parosi->offset.x), abs(parosi->offset.y));
					if (y - radius >= 0 && y + radius < input.height) {
						Range xRange = Range(fmax(radius, validXRange.start), fmin(input.width - radius, validXRange.end));
						for (int x = xRange.start; x < xRange.end; ++x) {
							double before = input.get(x - parosi->offset.x, y - parosi->offset.y);
							double at = input.get(x, y);
							double after = input.get(x + parosi->offset.x, y + parosi->offset.y);
							double strength = at - fmax(before, after);
							if (strength > 0) {
								orientations.add(x, y, parosi->orientation.multiply(strength));
							}
						}
					}
				}
			}
		}
	}
	return orientations;
}
PointMap FingerprintTemplate::blockOrientations(PointMap orientation, BlockMap blocks, BooleanMap mask) {
	PointMap sums = PointMap(blocks.blockCount.x, blocks.blockCount.y);
	for (int blockY = 0; blockY < blocks.blockCount.y; ++blockY) {
		for (int blockX = 0; blockX < blocks.blockCount.x; ++blockX) {
			Cell block = Cell(blockX, blockY);
			if (mask.get(block)) {
				Block area = blocks.blockAreas.get(block);
				for (int y = area.bottom(); y < area.top(); ++y) {
					for (int x = area.left(); x < area.right(); ++x) {
						sums.add(block, orientation.get(x, y));
					}
				}
			}
		}
	}
	return sums;
}
PointMap FingerprintTemplate::smoothOrientation(PointMap orientation, BooleanMap mask) {
	Cell size = mask.size();
	PointMap smoothed = PointMap(size.x, size.y);
	for (int blockY = 0; blockY < size.y; ++blockY) {
		for (int blockX = 0; blockX < size.x; ++blockX) {
			Cell block = Cell(blockX, blockY);
			if (mask.get(block)) {
				Block neighbors = Block::around(block, FingerprintContext::orientationSmoothingRadius).interset(Block(0, 0, size.x, size.y));
				for (int ny = neighbors.bottom(); ny < neighbors.top(); ++ny) {
					for (int nx = neighbors.left(); nx < neighbors.right(); ++nx) {
						if (mask.get(nx, ny)) {
							smoothed.add(block, orientation.get(nx, ny));
						}
					}
				}
			}
		}
	}
	return smoothed;
}
DoubleMap FingerprintTemplate::orientationAngles(PointMap vectors, BooleanMap mask) {
	Cell size = mask.size();
	DoubleMap angles = DoubleMap(size.x, size.y);
	for (int cellY = 0; cellY < size.y; ++cellY) {
		for (int cellX = 0; cellX < size.x; ++cellX) {
			Cell block = Cell(cellX, cellY);
			if (mask.get(block)) {
				angles.set(block, Angle::atan(vectors.get(block)));
			}
		}
	}
	return angles;
}
DoubleMap FingerprintTemplate::orientationMap(DoubleMap image, BooleanMap mask, BlockMap blocks) {
	PointMap accumulated = pixelwiseOrientation(image, mask, blocks); // Checked.
	PointMap byBlock = blockOrientations(accumulated, blocks, mask); // Checked.
	PointMap smooth = smoothOrientation(byBlock, mask); // Checked.
	return orientationAngles(smooth, mask); // Checked.
}
std::vector< std::list<Cell> > FingerprintTemplate::orientedLines(OrientedLineParams args) {
	std::vector< std::list<Cell> > result = std::vector< std::list<Cell> >(args.resolution);
	for (int orientationIndex = 0; orientationIndex < args.resolution; ++orientationIndex) {
		std::list<Cell> line;
		line.push_back(Cell(0, 0));
		Point direction = Angle::toVector(Angle::bucketCenter(orientationIndex, 2 * args.resolution));
		for (double r = args.radius; r >= 0.5; r /= args.step) {
			Cell sample = direction.multiply(r).roundPoint();
			std::list<Cell> temp = line;
			std::list<Cell>::iterator itr;
			bool insertDataIntoLineList = true;
			for (itr = temp.begin(); itr != temp.end(); itr++) {
				if (itr->x == sample.x && itr->y == sample.y) {
					insertDataIntoLineList = false;
				}
			}
			if (insertDataIntoLineList) {
				line.push_back(sample);
				line.push_back(sample.negate());
			}
		}
		result[orientationIndex] = line;
	}
	return result;
}
DoubleMap FingerprintTemplate::smoothRidges(DoubleMap input, DoubleMap orientation, BooleanMap mask, BlockMap blocks, double angle, std::vector< std::list<Cell> > lines) {
	DoubleMap output = DoubleMap(input.size().x, input.size().y);
	for (int cellBlockY = 0; cellBlockY < blocks.blockCount.y; ++cellBlockY) {
		for (int cellBlockX = 0; cellBlockX < blocks.blockCount.x; ++cellBlockX) {
			Cell block = Cell(cellBlockX, cellBlockY);
			if (mask.get(block)) {
				std::list<Cell> line = lines[Angle::quantize(Angle::add(orientation.get(block.x, block.y), angle), lines.size())];
				std::list<Cell>::iterator itr;
				for (Cell linePoint : line) {
					Block target = blocks.blockAreas.get(block.x, block.y);
					Block source = target.move(linePoint).interset(Block(0, 0, blocks.pixelCount.x, blocks.pixelCount.y));
					target = source.move(linePoint.negate());
					for (int y = target.bottom(); y < target.top(); ++y) {
						for (int x = target.left(); x < target.right(); ++x) {
							output.add(x, y, input.get(x + linePoint.x, y + linePoint.y));
						}
					}
				}
				Block blockArea = blocks.blockAreas.get(block.x, block.y);
				for (int y = blockArea.bottom(); y < blockArea.top(); ++y) {
					for (int x = blockArea.left(); x < blockArea.right(); ++x) {
						output.multiply(x, y, 1.0 / line.size());
					}
				}
			}
		}
	}
	return output;
}
BooleanMap FingerprintTemplate::binarize(DoubleMap input, DoubleMap baseLine, BooleanMap mask, BlockMap blocks) {
	Cell size = input.size();
	BooleanMap binarized = BooleanMap(size.x, size.y);
	for (int cellBlockY = 0; cellBlockY < blocks.blockCount.y; ++cellBlockY) {
		for (int cellBlockX = 0; cellBlockX < blocks.blockCount.x; ++cellBlockX) {
			Cell block = Cell(cellBlockX, cellBlockY);
			if (mask.get(block)) {
				Block rect = blocks.blockAreas.get(block.x, block.y);
				for (int y = rect.bottom(); y < rect.top(); ++y) {
					for (int x = rect.left(); x < rect.right(); ++x) {
						if ((input.get(x, y) - baseLine.get(x, y)) > 0) {
							binarized.set(x, y, true);
						}
					}
				}
			}
		}
	}
	return binarized;
}
BooleanMap FingerprintTemplate::fillBlocks(BooleanMap mask, BlockMap blocks) {
	BooleanMap pixelized = BooleanMap(blocks.pixelCount.x, blocks.pixelCount.y);
	for (int blockY = 0; blockY < blocks.blockCount.y; blockY++) {
		for (int blockX = 0; blockX < blocks.blockCount.x; blockX++) {
			Cell block = Cell(blockX, blockY);
			if (mask.get(block)) {
				for (int offsetY = 0; offsetY < blocks.blockAreas.get(block).height; offsetY++) {
					for (int offsetX = 0; offsetX < blocks.blockAreas.get(block).width; offsetX++) {
						Cell pixel = Cell(blocks.blockAreas.get(block).x + offsetX, blocks.blockAreas.get(block).y + offsetY);
						pixelized.set(pixel, true);
					}
				}
			}
		}
	}
	return pixelized;
}
BooleanMap FingerprintTemplate::invert(BooleanMap binary, BooleanMap mask) {
	Cell size = binary.size();
	BooleanMap inverted = BooleanMap(size.x, size.y);
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			inverted.set(x, y, !binary.get(x, y) && mask.get(x, y));
		}
	}
	return inverted;	
}
BooleanMap FingerprintTemplate::shrinkMask(BooleanMap mask, int amount) {
	Cell size = mask.size();
	BooleanMap shrunk = BooleanMap(size.x, size.y);
	for (int y = amount; y < size.y - amount; ++y) {
		for (int x = amount; x < size.x - amount; ++x) {
			shrunk.set(x, y, mask.get(x, y - amount) && mask.get(x, y + amount) && mask.get(x - amount, y) && mask.get(x + amount, y));
		}
	}
	return shrunk;
}
BooleanMap FingerprintTemplate::cInnerMask(BooleanMap outer) {
	Cell size = outer.size();
	BooleanMap inner = BooleanMap(size.x, size.y);
	for (int y = 1; y < size.y - 1; ++y) {
		for (int x = 1; x < size.x - 1; ++x) {
			inner.set(x, y, outer.get(x, y));
		}
	}
	if (FingerprintContext::innerMaskBorderDistance >= 1)
		inner = shrinkMask(inner, 1);
	int total = 1;
	for (int step = 1; total + step <= FingerprintContext::innerMaskBorderDistance; step *= 2) {
		inner = shrinkMask(inner, step);
		total += step;
	}
	if (total < FingerprintContext::innerMaskBorderDistance)
		inner = shrinkMask(inner, (FingerprintContext::innerMaskBorderDistance - total));
	return inner;
}
BooleanMap FingerprintTemplate::removeCrosses(BooleanMap input) {
	Cell size = input.size();
	bool any = true;
	while (any) {
		any = false;
		for (int y = 0; y < size.y - 1; ++y) {
			for (int x = 0; x < size.x - 1; ++x) {
				if ((input.get(x, y) && input.get(x + 1, y + 1) && !input.get(x, y + 1) && !input.get(x + 1, y)) || (input.get(x, y + 1) && input.get(x + 1, y) && !input.get(x, y) && !input.get(x + 1, y + 1))) {
					input.set(x, y, false);
					input.set(x, y + 1, false);
					input.set(x + 1, y, false);
					input.set(x + 1, y + 1, false);
					any = true;
				}
			}
		}
	}
	return input;
}
BooleanMap FingerprintTemplate::cleanupBinarized(BooleanMap binary) {
	Cell size = binary.size();
	BooleanMap inverted = BooleanMap(binary);
	inverted.invert();
	BooleanMap islands = vote(inverted, FingerprintContext::binarizedVote);
	BooleanMap holes = vote(binary, FingerprintContext::binarizedVote);
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			binary.set(x, y, ((binary.get(x, y) && !islands.get(x, y)) || holes.get(x, y)));
		}
	}
	return removeCrosses(binary);
}
std::vector<FingerprintMinutia* > FingerprintTemplate::filterMaskMinutiae(BooleanMap mask) {
	std::vector<FingerprintMinutia* > filteredList;
	Cell arrow;
	for (FingerprintMinutia* minutia : minutiae) {
		arrow = Angle::toVector(minutia->direction).multiply(-FingerprintContext::maskDisplacement).roundPoint();
		if (mask.get(minutia->position.plus(arrow), false)) {
			filteredList.push_back(minutia);
		}
	}
	return filteredList;
}
std::set<FingerprintMinutia* > FingerprintTemplate::filterRemoveMinutiaClouds() {
	int radiusSq = Integers::sq(FingerprintContext::minutiaCloudRadius);
	std::set<FingerprintMinutia* > minutiaSet;
	for (FingerprintMinutia* minutia : minutiae) {
		int count = 0;
		for (FingerprintMinutia* neighbour : minutiae) {
			if (neighbour->position.minus(minutia->position).lengthSq() <= radiusSq)
				count++;
		}
		
		if (FingerprintContext::maxCloudSize < (count - 1))
			minutiaSet.insert(minutia);
	}
	return minutiaSet;
}
std::vector<FingerprintMinutia* > FingerprintTemplate::filterRemoveMinutiaClouds(std::set<FingerprintMinutia* > removed) {
	std::vector<FingerprintMinutia* > minutiaList;
	for (FingerprintMinutia* minutia : minutiae) {
		if (!Utils::contains(removed, minutia)) {
			minutiaList.push_back(minutia);
		}
	}
	return minutiaList;
}
std::vector<FingerprintMinutia* > FingerprintTemplate::sortAndLimit_LimitTemplateSize() {
	std::list<int> mapKeyList;
	std::list<int> sortingList;
	std::vector<FingerprintMinutia* > minutiaList(minutiae);// = minutiae;
	std::map<int, FingerprintMinutia* > minutiaMap;
	for (FingerprintMinutia* minutia : minutiaList) {
		mapKeyList = std::list<int>();
		for (FingerprintMinutia* neighbor : minutiaList) {
			mapKeyList.push_back(minutia->position.minus(neighbor->position).lengthSq());
		}
		mapKeyList.sort();
		for (int i = 0; i < FingerprintContext::sortByNeighbor; i++) {
			mapKeyList.pop_front();
		}
		int finalValue;
		if (mapKeyList.size() > 0) {
			finalValue = mapKeyList.front();
		} else {
			finalValue = INT8_MAX;
		}
		sortingList.push_back(finalValue);
		minutiaMap.insert(std::pair<int, FingerprintMinutia* >(finalValue, minutia));
	}
	if (sortingList.size() > 0) {
		sortingList.sort();
		std::reverse(sortingList.begin(), sortingList.end());
		std::list<FingerprintMinutia* > sortedMinutiaeList;
		for (int value : sortingList) {
			sortedMinutiaeList.push_back(minutiaMap.find(value)->second);
		}
		int size = sortedMinutiaeList.size();
		if (size > FingerprintContext::maxMinutiae) {
			for (int i = FingerprintContext::maxMinutiae; i < size; i++) {
				std::list<FingerprintMinutia* >::iterator itr = sortedMinutiaeList.begin();
				std::advance(itr, FingerprintContext::maxMinutiae);
				sortedMinutiaeList.erase(itr);
			}
		}
		return std::vector<FingerprintMinutia* >(std::begin(sortedMinutiaeList), std::end(sortedMinutiaeList));
	}
	return minutiae;
}
void FingerprintTemplate::collectMinutiae(FingerprintSkeleton skeleton, const MinutiaType type) {
	for (SkeletonMinutia* m : skeleton.minutiae) {
		if (m->considered) {
			if (m->ridges.size() == 1) {
				minutiae.push_back(new FingerprintMinutia(m->position, m->ridges[0]->direction(), type));
			}
		}
	}
}
void FingerprintTemplate::maskMinutiae(BooleanMap mask) {
	minutiae = filterMaskMinutiae(mask);
}
void FingerprintTemplate::removeMinutiaClouds() {
	std::set<FingerprintMinutia* > removed = filterRemoveMinutiaClouds();
	minutiae = filterRemoveMinutiaClouds(removed);
}
void FingerprintTemplate::limitTemplateSize() {
	if (minutiae.size() > FingerprintContext::maxMinutiae) {
		minutiae = sortAndLimit_LimitTemplateSize();	
	}
}
void FingerprintTemplate::shuffleMinutiae() {
	std::random_shuffle(minutiae.begin(), minutiae.end());
}
std::vector<unsigned char> FingerprintTemplate::streamIso(std::vector<FingerprintMinutia* > fingerprintMinutiae, int bitmapWidth, int bitmapHeight) {
	std::vector<unsigned char> read;
	int count = fingerprintMinutiae.size();
	int extraData = 0;
	read.push_back((int) 'F');
	read.push_back((int) 'M');
	read.push_back((int) 'R');
	read.push_back((int) '\0');
	read.push_back((int) ' ');
	read.push_back((int) '2');
	read.push_back((int) '0');
	read.push_back((int) '\0');
	Utils::intToBytes(&read, (28 + (6 * count) + 2 + extraData));
	Utils::shortToBytes(&read, 0);
	Utils::shortToBytes(&read, (short) bitmapWidth);
	Utils::shortToBytes(&read, (short) bitmapHeight);
	Utils::shortToBytes(&read, (short) 197);
	Utils::shortToBytes(&read, (short) 197);
	read.push_back((int) '1');
	read.push_back((int) '0');
	read.push_back((int) '0');
	read.push_back((int) '0');
	read.push_back(86);
	read.push_back(count);
	for (int i = 0; i < count; i++) {
		int type = 1;
		if (fingerprintMinutiae[i]->type == MinutiaType::BIFURCATION) {
			type = 2;
		}
		int packedX = ((((short) type << 14) & 0xC000) | (((short) fingerprintMinutiae[i]->position.x) & 0x3fff));
		int packedY = ((((short) type << 14) & 0xC000) | (((short) (bitmapHeight - fingerprintMinutiae[i]->position.y)) & 0x3fff));
		Utils::shortToBytes(&read, (short) packedX);
		Utils::shortToBytes(&read, (short) packedY);
		double angle = (fingerprintMinutiae[i]->direction * 256.0 / Angle::PI2);
		read.push_back((int) angle);
		read.push_back((int) 0);
	}
	Utils::shortToBytes(&read, (short) 0);
	return read;
}
std::vector<FingerprintMinutia* >  FingerprintTemplate::parseIso(std::vector<unsigned char> minutia) {
	int index = 0;
	// 4B magic header "FMR\0"
    // 4B version " 20\0"
	// 4B template length in bytes (should be 28 + 6 * count + 2 + extra-data)
    // 2B junk
	index = 14;
	int width = Utils::readUnsignedShort(&index, minutia);
	int height = Utils::readUnsignedShort(&index, minutia);
	int xPixelsPerCM = Utils::readShort(&index, minutia);
    int yPixelsPerCM = Utils::readShort(&index, minutia);
    double dpiX = xPixelsPerCM * 255 / 100.0;
    double dpiY = yPixelsPerCM * 255 / 100.0;
    // 1B number of fingerprints in the template (assuming 1)
	// 1B junk
	// 1B junk
	// 1B junk
	// 1B fingerprint quality
    index = index + 5;
    int count = Utils::readUnsignedByte(&index, minutia);
    std::vector<FingerprintMinutia* > localMinutiae;
    for (int i = 0; i < count; ++i) {
    	// X position, upper two bits are type
        int packedX = Utils::readUnsignedShort(&index, minutia);
        // Y position, upper two bits ignored
        int packedY = Utils::readUnsignedShort(&index, minutia);
        // angle, 0..255 equivalent to 0..2pi
        int angle = Utils::readUnsignedByte(&index, minutia);
        // 1B minutia quality
        index = index + 1;
        // type: 01 ending, 10 bifurcation, 00 other (treated as ending)
        int type = (packedX >> 14) & 0x3;
        int x = packedX & 0x3fff;
        int y = packedY & 0x3fff;
        localMinutiae.push_back(new FingerprintMinutia(Cell(x, y), (angle * Angle::PI2 / 256.0), (type == 2 ? MinutiaType::BIFURCATION : MinutiaType::ENDING)));
	}
	// extra data length
    int extra = Utils::readUnsignedShort(&index, minutia);
    // variable-length extra data section
    index = index + extra;
    return localMinutiae;
}
