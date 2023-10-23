#pragma once
#include "RunnableService.h"
#include "SyncQueue.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
//using namespace cv;
//using namespace std;

class FrameSaver : RunnableService
{
	const double fps = 30;

	int width; 
	int height;
	cv::Size* frameSize;
	cv::VideoWriter* writer;
	SyncQueue<char>* inputQueue;
public:
	FrameSaver(SyncQueue<char>* inputQueue, const char* filename, int width, int height, CALLBACK(onCompleted));
	void Run() override;
	void Stop() override;
};





