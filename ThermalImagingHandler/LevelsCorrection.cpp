#include "LevelsCorrection.h"

pixel_8bit LevelsCorrection::GammaCorrection(double value, pixel_16bit& pixel)
{
	return (pixel_8bit)(255 * powl(value, gamma * (1 - speedEffect * speedLevels[pixel - min])));
}

void LevelsCorrection::GetBounds()
{
	min = 0xFFFF;
	max = 0x0000;
	for (size_t i = 0; i < size; i++)
	{
		if (current[i] < min) min = current[i];
		if (current[i] > max) max = current[i];
	}
	levelsSize = max - min + 1;
}

void LevelsCorrection::GetLevels()
{
	if (levels != nullptr) {
		delete levels;
	}

	levels = new pixel_16bit[levelsSize];
	for (size_t i = 0; i < levelsSize; i++)
	{
		levels[i] = 0;
	}
	for (size_t i = 0; i < size; i++)
	{
		levels[current[i] - min]++;
	}

	maxLevel = 0x0000;
	for (size_t i = 0; i < levelsSize; i++)
	{
		if (levels[i] > maxLevel) maxLevel = levels[i];
	}
}

void LevelsCorrection::GetSpeedLevels()
{
	if (speedLevels != nullptr) {
		delete speedLevels;
	}

	speedLevels = new double[levelsSize];
	for (size_t i = 0; i < levelsSize - 1; i++)
	{
		speedLevels[i] = abs(levels[i+1] - levels[i]);
	}
	speedLevels[levelsSize - 1] = speedLevels[levelsSize - 2];

	double maxSpeed = 0;
	for (size_t i = 0; i < levelsSize; i++)
	{
		if (speedLevels[i] > maxSpeed) maxSpeed = speedLevels[i];
	}

	for (size_t i = 0; i < levelsSize; i++)
	{
		speedLevels[i] /= maxSpeed;
	}

}

void LevelsCorrection::CreateIntervals()
{
	intervals.clear();
	int boundary = (int)(maxLevel * filterNoize);
	int pitLength = (int)(levelsSize * pitSize);
	bool started = false;
	for (size_t i = 0; i < levelsSize; i++)
	{
		if (started)
		{
			if (levels[i] <= boundary) continue;
			else
			{
				intervals.back()->Close(i-1);
				if (intervals.back()->length <= pitLength) {
					intervals.pop_back();
				}
				started = !started;
			}
		}
		else
		{
			if (levels[i] <= boundary)
			{
				intervals.push_back(new Interval(i));
				started = !started;
			}
			else continue;
		}
	}
	if (intervals.empty()) return;

	if (intervals.back()->length == -1 ) {
		intervals.back()->Close(levelsSize-1);
	} 
	if (intervals.back()->length <= pitLength) {
		intervals.pop_back();
	}

	double intervalsLength = 0;
	for (Interval* interval : intervals)
	{
		intervalsLength += interval->length;
	}
	this->intervalsLength = levelsSize - intervalsLength;

	int lastEnd = 0;
	double lastValue = 0;
	for (Interval* interval : intervals)
	{
		interval->value = lastValue + (interval->start - lastEnd) / (this->intervalsLength);
		lastEnd = interval->end+1;
		lastValue = interval->value;
	}
}

void LevelsCorrection::ConvertValue(pixel_16bit* rawPixel, pixel_8bit* pixel)
{
	if (*rawPixel == max) {
		*pixel = 255;
		return;
	}
	if(*rawPixel == min) {
		*pixel = 0;
		return;
	}

	int lastEnd = 0;
	double lastValue = 0;
	char state = 0;
	for (Interval* interval : intervals)
	{
		state = interval->Compare(*rawPixel - min);
		if (state == 0)
		{
			*pixel = GammaCorrection(interval->value, *rawPixel);
			return;
		}
		else if (state < 0)
		{	
			break;
		}
		lastEnd = interval->end + 1;
		lastValue = interval->value;
	}

	double value = lastValue + (*rawPixel - min - lastEnd) / intervalsLength;
	*pixel = GammaCorrection(value, *rawPixel);
}

LevelsCorrection::LevelsCorrection(int width, int heigth, double gamma) : width{ width }, height{ height }, gamma{ gamma }
{
	size = width * heigth;
	intervals = list<Interval*>();
}

pixel_8bit* LevelsCorrection::To8bit(pixel_16bit* rawFrame)
{
	current = rawFrame;
	GetBounds();
	GetLevels();
	GetSpeedLevels();
	CreateIntervals();

	pixel_8bit* frame = new pixel_8bit[size];
	if (!intervals.empty()) {
		for (size_t i = 0; i < size; i++)
		{
			ConvertValue(&rawFrame[i], &frame[i]);
		}
	}
	else
	{
		for (size_t i = 0; i < size; i++)
		{
			frame[i] = GammaCorrection((rawFrame[i] - min) / (double)levelsSize, rawFrame[i]);
		}
	}

	return frame;
}