#pragma once
#include <Shared/DynamicArray.h>
#include "ANet/Server.h"
#include "s_player.h"
#include "Shared/Network/NetEntity.h"

class Enviroment
{
public:
	DynamicArray<NetEntity*> entities;
	DynamicArray<Player*> players;

	void Update()
	{
		for (NetEntity* ent : entities)
		{
			ent->_update();
		}
	}

	int addPlayer(sockaddr_in address, int classId, int weaponId)
	{
		// TODO: REMEMBER TO FREE!
		Player* p = new Player(++Player::ID, { 400.0f, 400.0f }, { 0.0f, 0.0f });
		p->client = address;
		p->classId = classId;
		p->weaponId = weaponId;

		entities.pushBack(p);
		players.pushBack(p);

		return p->id;
	}

	void movePlayer(int id, InputStruct& inputs, int size)
	{
		for (Player* p : players)
		{
			if (p->id != id)
				continue;

			if (inputs.w)
				p->position.value.y += 1.0f;
			if (inputs.s)
				p->position.value.y -= 1.0f;
			if (inputs.a)
				p->position.value.x -= 1.0f;
			if (inputs.d)
				p->position.value.x += 1.0f;
		}
	}
};