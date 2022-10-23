#pragma once
#include "ANet/Client.h"
#include "Shared/NetClock.h"
#include "Shared/DynamicArray.h"

#include "cEntityHandler.h"
#include "cRocket.h"
#include "cPlayer.h"

class World
{
public:
	NetClient* client;
	NetClock clock;
	EntityHandler entityHandler;

	World(NetClient* net);
};