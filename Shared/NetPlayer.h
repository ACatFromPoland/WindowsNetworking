#pragma once
#include "NetEntity.h"

class NetPlayer : public NetEntity
{
public:
	const static int maxPlayers = 128;

	NetVector2(position);
	NetVector2(velocity);

	virtual void update() = 0;

	NetPlayer()
	{
		
	}
};