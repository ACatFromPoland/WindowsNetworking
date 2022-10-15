#pragma once
#include "Shared/Network/NetPlayer.h"

class Player : public NetPlayer
{
public:
	static int ID;

	sockaddr_in client;
	
	Player(int id, Vector2 pos, Vector2 vel);

	virtual void _update();
};
