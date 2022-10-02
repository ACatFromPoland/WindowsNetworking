#pragma once

const int maxPlayerCount = 8;

class Player
{
public:
	bool active;
	sockaddr_in client;
	float x;
	float y;

	Player()
	{
		active = false;
		client = {};
		x = 0.0f;
		y = 0.0f;
	}
};

static Player players[maxPlayerCount];

// TODO: Intergrate SF::Vectors
int addPlayer(sockaddr_in& address)
{
	for (int i = 0; i < maxPlayerCount; i++)
	{
		Player& player = players[i];
		
		if (player.active)
			continue;

		player.active = true;
		player.x = 400.0f;
		player.y = 400.0f;
		return i;
	}
	return -1;
}

void movePlayer(int id, bool inputs[])
{
	Player& player = players[id];
	if (inputs[0]) // W
	{
		player.y -= 1;
	}
	if (inputs[1]) // A
	{
		player.x -= 1;
	}
	if (inputs[2]) // S
	{
		player.y += 1;
	}
	if (inputs[3]) // D
	{
		player.x += 1;
	}
}
