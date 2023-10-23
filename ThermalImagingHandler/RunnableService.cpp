#include "RunnableService.h"

RunnableService::RunnableService(CALLBACK(onCompleted)) {
	isActive = false;
	isInputClosed = false;
	this->onCompleted = onCompleted;
}

void RunnableService::Stop() {
	isActive = false;
}

void RunnableService::InputClosed() {
	isInputClosed = true;
}