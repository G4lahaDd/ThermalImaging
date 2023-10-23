#pragma once
class Interval
{
public:
	int start;
	int end;
	int length;
	int value;

	Interval(int start);
	int Close(int end);
	char Compare(int point);
};

