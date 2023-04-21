#pragma once
#include <SDL/SDL.h>

//github shakesoda
//time is in seconds
struct TimeData {
	Uint64 then;
	Uint64 now;
	Uint64 frequency;
	double delta;
};

class Timer {
public:
	Timer();
	// Note: delta is since last touch OR peek
	const TimeData& touch();
	// Note: delta is since last touch
	const TimeData& peek();

protected:
	TimeData m_time;
};