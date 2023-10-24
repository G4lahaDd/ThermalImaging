#include "FrameController.h"
#include <conio.h>
#include "SyncQueue.h"

int frameHeight = 512;
int frameWidth = 640;
double gamma = 2.2;

int main(int argc, char* argv[])
{
	cout << "Press any button to start" << endl;
	_getch();
	cout << "START" << endl;
	cout << "Press any button to stop" << endl;

	FrameController* controller = nullptr;
	try {
		controller = new FrameController(frameWidth, frameHeight, gamma, "C:\\Users\\ilya-\\Videos\\Captures\\Result.mp4");
		controller->Start("C:\\Users\\ilya-\\Downloads\\dump_13122019_145433.bin");
	}
	catch (const char* message) {
		cout << message << endl;
	}
	catch (...) {
		cout << "Error while initializing frame controller" << endl;
	}

	_getch();

	if (controller != nullptr && controller->IsActive())
	{
		controller->Stop();
	}
	cout << "END" << endl;
}