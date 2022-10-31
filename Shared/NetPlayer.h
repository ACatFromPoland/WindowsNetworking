#pragma once
#include "NetEntity.h"

class NetPlayer : public Entity
{
public:
	const static int maxPlayers = 128;
	
	NetByte(classType);
	NetVector2(mousePosition);
	NetFloat(health);

	NetPlayer()
	{
		
	}	
};