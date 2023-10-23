#pragma once
#define CALLBACK(name) std::function<void()> name
#include <functional>

using namespace std;

class RunnableService
{
public:
	RunnableService(CALLBACK(onCompleted));

	virtual void Run() = 0;

	virtual void Stop();

	void InputClosed();
protected:
	bool isActive;
	bool isInputClosed;
	//void (*onCompleted)();
	CALLBACK(onCompleted);
};

