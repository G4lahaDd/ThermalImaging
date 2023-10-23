#include "FrameProcessor.h"

FrameProcessor::FrameProcessor(SyncQueue<pixel_16bit>* inputQueue, SyncQueue<pixel_8bit>* outputQueue, double gamma, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, inputQueue{ inputQueue }, outputQueue{outputQueue}, gamma { gamma }, height{ height }, width{ width }
{
	frameSize = width * height;
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
		frame[i] = (pixel_8bit)(powl((rawFrame[i] - bounds[0]) / (bounds[1] - bounds[0]), gamma) * 255);
	}
	return frame;
}

void FrameProcessor::Run() {
	isActive = true;
	while (isActive && !(isInputClosed && inputQueue->Empty()))
	{
		pixel_16bit* rawFrame = inputQueue->Dequeue();
		if (rawFrame == nullptr) continue;

		pixel_8bit* frame = ProcessFrame(rawFrame);
		if (frame != nullptr) outputQueue->Enqueue(frame);

		delete rawFrame;
	}
	onCompleted();
}