#include "FrameProcessor.h"
#include <opencv2/opencv.hpp>

FrameProcessor::FrameProcessor(SyncQueue<pixel_16bit>* inputQueue, SyncQueue<pixel_8bit>* outputQueue, double gamma, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, inputQueue{ inputQueue }, outputQueue{outputQueue}, gamma { gamma }, height{ height }, width{ width }
{
	frameSize = width * height;
	levels = new LevelsCorrection(width, height, gamma);
}

pixel_16bit* FrameProcessor::GetBounds(pixel_16bit* rawFrame) {
	pixel_16bit* bounds = new pixel_16bit[2];//min & max
	bounds[0] = 0xFFFF;//min
	bounds[1] = 0x0000;//max
	for (size_t i = 0; i < frameSize; i++)
	{
		if (rawFrame[i] < bounds[0]) bounds[0] = rawFrame[i];
		if (rawFrame[i] > bounds[1]) bounds[1] = rawFrame[i];
	}
	return bounds;
}

pixel_8bit* FrameProcessor::ProcessFrame(pixel_16bit* rawFrame) {
	pixel_16bit* bounds = GetBounds(rawFrame);
	pixel_8bit* frame = new pixel_8bit[frameSize];
	for (size_t i = 0; i < frameSize; i++)
	{
		frame[i] = (pixel_8bit)(powl((rawFrame[i] - bounds[0]) / (double)(bounds[1] - bounds[0]), gamma) * 255);
	}
	return frame;
}

void FrameProcessor::TEST(pixel_16bit* rawFrame) {
	pixel_16bit* bounds = GetBounds(rawFrame);
	pixel_8bit* frame = new pixel_8bit[frameSize];
	pixel_8bit* bbframe = new pixel_8bit[frameSize];
	for (size_t i = 0; i < frameSize; i++)
	{
		frame[i] = (pixel_8bit)(powl((rawFrame[i] - bounds[0]) / (double)(bounds[1] - bounds[0]), gamma) * 255);
	}

	int r = bounds[1] - bounds[0] + 1;
	pixel_16bit* levels = new pixel_16bit[r];
	for (size_t i = 0; i < r; i++)
	{
		levels[i] = 0;
	}
	for (size_t i = 0; i < frameSize; i++)
	{
		levels[rawFrame[i] - bounds[0]]++;
	}

	size_t maxLevel = 0x0000;
	for (size_t i = 0; i < r; i++)
	{
		if (levels[i] > maxLevel) maxLevel = levels[i];
	}
	int per = 10;
	pixel_8bit* levelsFrameData = new pixel_8bit[r / per * 1024];
	for (int i = 0; i < r/per; i++)
	{
		pixel_16bit level = (pixel_16bit)(1023 * (levels[i*per] / (double)maxLevel));
		for (int j = 0; j < level; j++)
		{
			levelsFrameData[i * 1024 + j] = 0xFF;
		}
	}
	
	cv::Mat levelsFrame(cv::Size(1024,r / per), CV_8U, levelsFrameData);
	cv::Mat proccessedFrame(cv::Size(width, height), CV_8U, frame);
	cv::imshow("Levels", levelsFrame);
	cv::imshow("Result", proccessedFrame);

	cv::waitKey(0);

	delete rawFrame;
}

void FrameProcessor::Run() {
	cout << "[INFO] PROCESSOR: START\n";
	isActive = true;
	int counter = 0;
	while (isActive && !(isInputClosed && inputQueue->Empty()))
	{
		pixel_16bit* rawFrame = inputQueue->Dequeue();
		if (rawFrame == nullptr) continue;

		counter++;
		cout << "[INFO] FRAME " << counter;
		//pixel_8bit* frame = ProcessFrame(rawFrame);
		pixel_8bit* frame = levels->To8bit(rawFrame);
		if (frame != nullptr) outputQueue->Enqueue(frame);
		
		delete rawFrame;
	}
	onCompleted();
	cout << "[INFO] PROCESSOR: END\n";
}

