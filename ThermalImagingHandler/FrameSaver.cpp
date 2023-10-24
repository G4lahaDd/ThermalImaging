#include "FrameSaver.h"
#include <iostream>

#pragma comment(lib, "opencv_world480.lib")
#pragma comment(lib, "opencv_world480d.lib")

FrameSaver::FrameSaver(SyncQueue<char>* inputQueue, const char* filename, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, inputQueue{ inputQueue }, height{ height }, width{ width }
{
	frameSize = cv::Size(width, height);
	cout << "[INFO] SAVER:OPEN\n";

	writer = new cv::VideoWriter(filename, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, frameSize, true);
	//writer = new cv::VideoWriter(filename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frameSize, false);
	cout << "[INFO] SAVER: state " << writer->isOpened() << endl;
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
	cout << "[INFO] SAVER: START\n";
	isActive = true;
	while (isActive && !(isInputClosed && inputQueue->Empty()))
	{
		char* frameData = inputQueue->Dequeue();
		if (frameData == nullptr) continue;

		char* rgbData = new char[height * width * 3];
		for (size_t i = 0; i < height * width; i++)
		{
			rgbData[3 * i] = frameData[i];
			rgbData[3 * i + 1] = frameData[i];
			rgbData[3 * i + 2] = frameData[i];
		}
		cv::Mat frame = cv::Mat(cv::Size(width, height), CV_8UC3, rgbData);
		cv::imshow("Result", frame);
		cv::waitKey(33);
		writer->write(frame);
	}
	onCompleted();
	cout << "[INFO] SAVER: END\n";
}