#pragma once
#include "ANet/Server.h"
#include "Shared/NetPlayer.h"
#include "Shared/NetPackets.h"
#include <stdio.h>

class Player : public NetPlayer
{
public:
	double lastPacketTick;
	sockaddr_in client;

	bitInt inputs;
	Vector2 mousePosition;
	double lastClickTick;

	Player()
	{
		type = EntityTypes::ENT_PLAYER;
	}

	virtual bool isPlayer() { return true; }
	
	virtual void update() 
	{
		if (inputs.get(IN_FORWARD)) position.value.y -= 0.0001f;
		if (inputs.get(IN_LEFT)) position.value.x -= 0.0001f;
		if (inputs.get(IN_BACK)) position.value.y += 0.0001f;
		if (inputs.get(IN_RIGHT)) position.value.x += 0.0001f;
	}

	void setInputs(bitInt in, Vector2 mousePos)
	{
		inputs = in;
		mousePosition = mousePos;
	}
};