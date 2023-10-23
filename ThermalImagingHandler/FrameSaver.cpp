#include "FrameSaver.h"
#include <iostream>

FrameSaver::FrameSaver(SyncQueue<char>* inputQueue, const char* filename, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, inputQueue{ inputQueue }, height{ height }, width{ width }
{
	frameSize = new cv::Size(width, height);
	cout << *frameSize << endl;
	printf_s("[INFO] SAVER:OPEN\n");

	//writer = new cv::VideoWriter(filename, CV_FOURCC('m', 'p', '4', 'v'), fps, *frameSize, false);
	writer = new cv::VideoWriter("C:\\Users\\ilya-\\Videos\\Captures\\output_video.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(640, 512), false);
	//writer->open(filename, CV_FOURCC('m', 'p', '4', 'v'), fps, *frameSize, false);
	cout << *frameSize << endl;
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

		cv::Mat* frame = new cv::Mat(height, width, CV_8UC(1), frameData);
		writer->write(*frame);

		printf_s("[INFO] SAVER: frame been saved\n");
	}
	onCompleted();
	printf_s("[INFO] SAVER: END\n");
}