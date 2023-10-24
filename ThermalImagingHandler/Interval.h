#pragma once
class Interval
{
public:
	int start;
	int end;
	int length;
	double value;

	Interval(int start);
	int Close(int end);
	char Compare(int point);
};

