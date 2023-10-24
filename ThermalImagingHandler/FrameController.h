#pragma once
#include "FrameReader.h"
#include "FrameProcessor.h"
#include "FrameSaver.h"
#include <thread>

using namespace std;

class FrameController
{
	int width;
	int height;
	double gamma;
	const char* outputFilename;

	bool isActive;
	RunnableService* saver;
	RunnableService* processor;
	RunnableService* reader;

	std::thread* readerThread;
	std::thread* processorThread;
	std::thread* saverThread;

public:
	FrameController(int width, int height, double gamma, const char* outputFilename);
	bool Start(const char* path);
	void OnEnd();
	void Stop();
	bool IsActive();
};





