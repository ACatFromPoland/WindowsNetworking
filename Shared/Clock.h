#pragma once
#include <time.h>

class NetClock
{
public:
	NetClock()
	{
		clock_t currentTick = clock();
		clock_t lastTick = currentTick;
		time = 0.0f;
		period = 1;
	}
	NetClock(double ticksPerSecond)
	{
		clock_t currentTick = clock();
		clock_t lastTick = currentTick;
		time = 0.0f;
		period = 1.0f / ticksPerSecond;
	}

	bool Tick()
	{
		currentTick = clock();
		time += (double)(currentTick - lastTick);
		lastTick = currentTick;
		if (time > (double)(0.015 * CLOCKS_PER_SEC))
		{
			time -= (double)(0.015 * CLOCKS_PER_SEC);
			return true;
		}
		return false;
	}

private:
	clock_t currentTick;
	clock_t lastTick;
	double time;
	double period;
};