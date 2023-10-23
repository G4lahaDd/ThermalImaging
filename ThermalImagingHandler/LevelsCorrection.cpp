#include "LevelsCorrection.h"

pixel_8bit LevelsCorrection::GammaCorrection(double value)
{
	return (pixel_8bit)(255 * powl(value, gamma));
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
				intervals.back()->Close(i);
				if (intervals.back()->length < pitLength) {
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
		lastEnd = interval->end;
		lastValue = interval->value;
	}
}

void LevelsCorrection::ConvertValue(pixel_16bit* rawPixel, pixel_8bit* pixel)
{
	int lastEnd = 0;
	double lastValue = 0;
	char state = 0;
	for (Interval* interval : intervals)
	{
		state = interval->Compare(*rawPixel - min);
		if (state == 0)
		{
			*pixel = (pixel_8bit)(interval->value * 255);
			return;
		}
		else if (state < 0)
		{
			break;
		}
		lastEnd = interval->end;
		lastValue = interval->value;
	}

	*pixel = GammaCorrection(lastValue + (*rawPixel - min - lastEnd) / intervalsLength);
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
	CreateIntervals();

	//Create new frame
	pixel_8bit* frame = new pixel_8bit[size];
	std::cout << " Intervals: " << intervals.size() << endl;
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
			frame[i] = GammaCorrection((rawFrame[i] - min) / (double)levelsSize);
		}
	}

	return frame;
}