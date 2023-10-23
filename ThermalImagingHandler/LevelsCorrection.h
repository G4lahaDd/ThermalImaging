#pragma once
#include <iostream>
#include <list>
#include "Interval.h"

#define pixel_16bit unsigned short
#define pixel_8bit char

using namespace std;

class LevelsCorrection
{
	const double filterNoize = 0.005;//������� ������������� ���������� �������� �� ����
	const double pitSize = 0.20;//����������� ������ � ��������� ��� ������� ��������
	
	double gamma;

	pixel_16bit* current;
	int width;
	int height;
	int size;

	pixel_16bit* levels;
	pixel_16bit levelsSize;
	pixel_16bit maxLevel;
	pixel_16bit min;
	pixel_16bit max;
	
	list<Interval*> intervals;
	double intervalsLength;

	pixel_8bit GammaCorrection(double value);
	void GetBounds();
	void GetLevels();
	void CreateIntervals();
	void ConvertValue(pixel_16bit* rawPixel, pixel_8bit* pixel);
public:
	LevelsCorrection(int width, int heigth, double gamma);
	pixel_8bit* To8bit(pixel_16bit* rawFrame);
};

