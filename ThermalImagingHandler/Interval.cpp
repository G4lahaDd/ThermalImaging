#include "Interval.h"

Interval::Interval(int start) : start{ start }, end{start}
{
	length = -1;
	value = 0;
}

int Interval::Close(int end) {
	this->end = end;
	length = end - start + 1;
	return length;
}

char Interval::Compare(int point) {
	if (point < start) return -1;
	if (point > end) return 1;
	return 0;
}