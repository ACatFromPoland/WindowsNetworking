#pragma once
#include "ANet/Server.h"
#include "Shared/NetPlayer.h"
#include "Shared/NetPackets.h"

class Player : public NetPlayer
{
public:
	int lastPacketID;
	double lastPacketTick;
	sockaddr_in client;
	bool bot;

	Player() : NetPlayer()
	{

	}

	void init(int _lastPacketID, sockaddr_in _client, int _id, Vector2 _position)
	{
		lastPacketID = _lastPacketID;
		client = _client;
		id = _id;
		position = _position;
		bot = false;
	}

	virtual void update()
	{

	}

	void movePlayer(bitInt inputs)
	{
		if (inputs.get(IN_FORWARD)) position.value.y -= 1.0f;
		if (inputs.get(IN_LEFT)) position.value.x -= 1.0f;
		if (inputs.get(IN_BACK)) position.value.y += 1.0f;
		if (inputs.get(IN_RIGHT)) position.value.x += 1.0f;
	}
};