#include "FrameReader.h"
#include <string.h>
#include <errno.h>
#include <iostream>

FrameReader::FrameReader(FILE* f, SyncQueue<pixel_16bit>* outputQueue, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, height{ height }, width{ width }, outputQueue{ outputQueue }, f{ f }
{
	frameSize = width * height;
	fseek(f, sizeof(char) * 32, SEEK_SET);
}

pixel_16bit* FrameReader::GetNext() {
	pixel_16bit* frame = new pixel_16bit[frameSize];
	size_t size = fread(frame, sizeof(pixel_16bit), frameSize, f);
	if (size != frameSize)
	{
		isActive = false;
		return nullptr;
	}
	return frame;
}

void FrameReader::Stop() {
	isActive = false;
	fclose(f);

}


void FrameReader::Run() {
	isActive = true;
	std::cout << "[INFO] READER: START\n";
	while (isActive && !feof(f))
	{
		pixel_16bit* frame = GetNext();
		if (frame != nullptr) outputQueue->Enqueue(frame);
	}
	onCompleted();
	Stop();
	std::cout << "[INFO] READER: END\n";
}