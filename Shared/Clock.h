#pragma once
#include <profileapi.h>

// TODO: REPLACE TIME GET TIME
class NetClock
{
public:
	NetClock()
	{
		Restart();
		period = 1000000.0f;
	}

	NetClock(double ticksPerSecond)
	{
		Restart();
		period = (1.0f / ticksPerSecond) * 1000000.0f;
	}

	void Restart()
	{
		QueryPerformanceCounter(&ticks);
		lastTicks = ticks;
		timeus = 0.0f;
	}

	bool Tick()
	{
		QueryPerformanceCounter(&ticks);
		timeus += (double)(ticks.QuadPart - lastTicks.QuadPart);
		lastTicks = ticks;

		if (timeus > period)
		{
			timeus -= period;
			return true;
		}
		return false;
	}

private:
	LARGE_INTEGER ticks;
	LARGE_INTEGER lastTicks;
	double timeus;
	double period;
};