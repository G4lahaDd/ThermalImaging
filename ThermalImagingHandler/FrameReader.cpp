#include "FrameReader.h"
#include <string.h>
#include <errno.h>


FrameReader::FrameReader(FILE* f, SyncQueue<pixel_16bit>* outputQueue, int width, int height, CALLBACK(onCompleted)) : RunnableService{ onCompleted }, height{ height }, width{ width }, outputQueue{ outputQueue }, f{ f }
{
	frameSize = width * height;
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
	while (isActive && !feof(f))
	{
		pixel_16bit* frame = GetNext();
		if (frame != nullptr) outputQueue->Enqueue(frame);
	}
	onCompleted();
	Stop();
}