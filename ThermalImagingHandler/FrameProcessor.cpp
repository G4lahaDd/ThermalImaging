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

void FrameProcessor::Run() {
	cout << "[INFO] PROCESSOR: START\n";
	isActive = true;
	int counter = 0;
	while (isActive && !(isInputClosed && inputQueue->Empty()))
	{
		pixel_16bit* rawFrame = inputQueue->Dequeue();
		if (rawFrame == nullptr) continue;

		counter++;
		if (counter % 100 == 0) {

			cout << "[INFO] PROCESS: " << counter / 10 << "%" << endl;
		}
		pixel_8bit* frame = levels->To8bit(rawFrame);
		if (frame != nullptr) outputQueue->Enqueue(frame);

		delete rawFrame;
	}
	onCompleted();
	cout << "[INFO] PROCESSOR: END\n";
}

