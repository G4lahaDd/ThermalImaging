#include "FrameController.h"
#include "SyncQueue.h"

FrameController::FrameController(int width, int height, double gamma, const char* outputFilename) : width{ width }, gamma{ gamma }, height{ height }, outputFilename{ outputFilename }
{}

void FrameController::OnEnd() {
	readerThread->join();
	processorThread->join();
}

void FrameController::Stop() {
	reader->Stop();
	processor->Stop();
	saver->Stop();

	readerThread->join();
	processorThread->join();
	processorThread->join();

	delete reader;
	delete processor;
	delete saver;
}

bool FrameController::Start(const char* path) {
	FILE* f;
	errno_t err = fopen_s(&f, path, "rb");
	if (err) {
		char* message = new char[128];
		strerror_s(message, sizeof(char) * 128, err);
		throw message;
	}

	SyncQueue<unsigned short>* rawFrames = new SyncQueue<unsigned short>();
	SyncQueue<char>* frames = new SyncQueue<char>();

	try {
		auto saverCallback = [this]() { this->OnEnd(); };
		saver = (RunnableService*)(new FrameSaver(frames, outputFilename, width, height, saverCallback));

		auto processorCallback = [this]() { saver->InputClosed(); };
		processor = (RunnableService*)(new FrameProcessor(rawFrames, frames, gamma, width, height, processorCallback));

		auto readerCallback = [this]() { processor->InputClosed(); };
		reader = (RunnableService*)(new FrameReader(f, rawFrames, width, height, readerCallback));


		saverThread = new std::thread([this]() { saver->Run(); });
		processorThread = new std::thread([this]() { processor->Run(); });
		readerThread = new std::thread([this]() { reader->Run(); });
	}
	catch (const char* message) {
		throw message;
	}
	catch (...) {
		throw "Error while initializing services";
	}

}