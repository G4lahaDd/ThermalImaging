#include "FrameController.h"
#include <conio.h>
#include "SyncQueue.h"

int frameHeight = 512;
int frameWidth = 640;
double gamma = 2.2;

int main(int argc, char* argv[])
{
	printf_s("START\n");
	try {
		FrameController* controller = new FrameController(frameWidth, frameHeight, gamma, "C:\\Users\\ilya-\\Videos\\Captures\\Result.avi");
		controller->Start("C:\\Users\\ilya-\\Downloads\\dump_13122019_145433.bin");
	}
	catch (const char* message) {
		printf_s(message);
	}
	catch (...) {
		printf_s("Error while initializing frame controller");
	}
	printf_s("WAITING RESULTS\n");
	_getch();
	printf_s("END\n");
}