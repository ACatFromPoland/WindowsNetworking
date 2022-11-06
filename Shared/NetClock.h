#pragma once
#include <Windows.h>
#include <profileapi.h>

class NetClock
{
public:
	NetClock();

	NetClock(double ticksPerSecond);

	void Restart();

	bool Tick();

	double getTick();

	double timeSince(double time);

private:
	LARGE_INTEGER ticks;
	LARGE_INTEGER lastTicks;
	double timeus;
	double period;
	double tickPerSecond;
};