#include "FrameSaver.h"
#include <iostream>

#pragma comment(lib, "opencv_world470.lib")
#pragma comment(lib, "opencv_world470d.lib")

FrameSaver::FrameSaver(SyncQueue<char>* inputQueue, const char* filename, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, inputQueue{ inputQueue }, height{ height }, width{ width }
{
	frameSize = cv::Size(width, height);
	cout << frameSize << endl;
	printf_s("[INFO] SAVER:OPEN\n");

	//writer = new cv::VideoWriter(filename, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, frameSize, false);
	//writer = new cv::VideoWriter(filename, cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, frameSize, false);
	writer = new cv::VideoWriter(filename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frameSize, false);
	cout << frameSize << endl;
	printf_s("[INFO] SAVER: state ");
	cout << writer->isOpened() << endl;
	if (!writer->isOpened()) {
		throw "[ERROR] SAVER: CAN NOT OPEN\n";
	}
}

void FrameSaver::Stop() {
	isActive = false;
	writer->release();
}

void FrameSaver::Run()
{
	printf_s("[INFO] SAVER: START\n");
	isActive = true;
	while (isActive && !(isInputClosed && inputQueue->Empty()))
	{
		char* frameData = inputQueue->Dequeue();
		if (frameData == nullptr) continue;

		cv::Mat frame = cv::Mat(cv::Size(width, height), CV_8U, frameData);
		writer->write(frame);
	}
	onCompleted();
	printf_s("[INFO] SAVER: END\n");
}