#include "cWorld.h"

World::World(NetClient* net)
{
	client = net;
	clock = NetClock(66.0f);	
}