#pragma once
#include <Shared/DynamicArray.h>
#include "ANet/Host.h"
#include "Player.h"

class Enviroment
{
public:
	DynamicArray<Entity*> entities;
	DynamicArray<Player*> players;

	void Update()
	{
		for (Entity* ent : entities)
		{
			ent->_update();
		}
	}

	int addPlayer(sockaddr_in address)
	{
		// TODO: REMEMBER TO FREE!
		Player* p = new Player(++Player::ID, { 400.0f, 400.0f }, { 0.0f, 0.0f });
		p->client = address;
		entities.pushBack(p);
		players.pushBack(p);

		return p->id;
	}

	void movePlayer(int id, bool* inputs, int size)
	{

	}
};