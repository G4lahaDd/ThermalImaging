#pragma once
#include "SyncQueue.h"
#include <corecrt_math.h>
#include "RunnableService.h"
#include "LevelsCorrection.h"

#define pixel_16bit unsigned short
#define pixel_8bit char


class FrameProcessor : RunnableService
{
	double gamma;
	int width;
	int height;
	size_t frameSize;
	SyncQueue<pixel_16bit>* inputQueue;
	SyncQueue<pixel_8bit>* outputQueue;
	LevelsCorrection* levels;

	pixel_16bit* GetBounds(pixel_16bit*);
public:
	FrameProcessor(SyncQueue<pixel_16bit>* inputQueue, SyncQueue<pixel_8bit>* outputQueue, double gamma, int width, int height, CALLBACK(onCompleted));
	void Run() override;
};
