#pragma once
#define pixel_16bit unsigned short
#include "RunnableService.h"
#include "SyncQueue.h"
#include <cstdio>

class FrameReader : RunnableService
{
	FILE* f;
	int width;
	int height;
	size_t frameSize;
	SyncQueue<pixel_16bit>* outputQueue;

	pixel_16bit* GetNext();
public:
	FrameReader(FILE* f, SyncQueue<pixel_16bit>* outputQueue, int width, int height, CALLBACK(onCompleted));
	void Run() override;
	void Stop() override;
};



