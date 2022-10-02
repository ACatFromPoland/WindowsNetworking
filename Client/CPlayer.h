#pragma once

//TODO: Make player array dynamic 
// Change up how players are identified 
const int maxPlayerCount = 8;

struct
{
	int id;
} LocalClient;

class Player
{
public:
	bool active;
	float x;
	float y;

	Player()
	{
		active = false;
		x = 0.0f;
		y = 0.0f;
	}
};

static Player players[maxPlayerCount];
