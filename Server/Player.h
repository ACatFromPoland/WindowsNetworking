#pragma once

const int invalidID = -1;
const int maxPlayers = 8;
const int inputTypes = 5;

class Player
{
public:
	sockaddr_in client;
	int id;
	float x;
	float y;

	static Player players[maxPlayers];

	Player()
	{
		client = {};
		id = invalidID;
		x = 0.0f;
		y = 0.0f;
	}
};

Player Player::players[maxPlayers];

int addPlayer(sockaddr_in& address)
{
	for (int i = 0; i < maxPlayers; i++)
	{
		Player& p = Player::players[i];

		if (p.id != invalidID)
			continue;

		p.id = i;
		p.client = address;
		p.x = 400.0f;
		p.y = 400.0f;
		return i;
	}
}

void movePlayer(int id, bool inputs[])
{
	Player& p = Player::players[id];
	if (inputs[0]) // W
	{
		p.y -= 1;
	}
	if (inputs[1]) // A
	{
		p.x -= 1;
	}
	if (inputs[2]) // S
	{
		p.y += 1;
	}
	if (inputs[3]) // D
	{
		p.x += 1;
	}
}
