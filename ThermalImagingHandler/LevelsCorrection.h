#pragma once
#include <iostream>
#include <list>
#include "Interval.h"

#define pixel_16bit unsigned short
#define pixel_8bit char

using namespace std;

class LevelsCorrection
{
	const double filterNoize = 0.0025;//Процент максимального отклонения значений от нуля
	const double pitSize = 0.10;//Минимальный размер в процентах ямы нулевых значений
	const double speedEffect = 0.3;

	double gamma;

	pixel_16bit* current;
	int width;
	int height;
	int size;

	pixel_16bit* levels;
	double* speedLevels;
	pixel_16bit levelsSize;
	pixel_16bit maxLevel;
	pixel_16bit min;
	pixel_16bit max;

	list<Interval*> intervals;
	double intervalsLength;

	pixel_8bit GammaCorrection(double value, pixel_16bit& pixel);
	void GetBounds();
	void GetLevels();
	void GetSpeedLevels();
	void CreateIntervals();
	void ConvertValue(pixel_16bit* rawPixel, pixel_8bit* pixel);
public:
	LevelsCorrection(int width, int heigth, double gamma);
	pixel_8bit* To8bit(pixel_16bit* rawFrame);
};

