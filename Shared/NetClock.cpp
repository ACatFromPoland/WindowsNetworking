#pragma once
#include "NetClock.h"
#include <iostream>

NetClock::NetClock()
{
	Restart();
	period = 10000000.0f;
}

NetClock::NetClock(double ticksPerSecond)
{
	Restart();
	period = (1.0f / ticksPerSecond) * 10000000.0f;
	tickPerSecond = ticksPerSecond;
}

void NetClock::Restart()
{
	QueryPerformanceCounter(&ticks);
	lastTicks = ticks;
	timeus = 0.0f;
}

bool NetClock::Tick()
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

double NetClock::getTick()
{
	LARGE_INTEGER tticks;
	QueryPerformanceCounter(&tticks);
	return ((double)tticks.QuadPart);
}

double NetClock::timeSince(double time)
{
	LARGE_INTEGER tticks;
	QueryPerformanceCounter(&tticks);
	return (((double)tticks.QuadPart) - time) / 10000000.0f;
}