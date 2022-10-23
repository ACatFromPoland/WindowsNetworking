#pragma once
#include "Shared/DynamicArray.h"
#include "Shared/NetShared.h"

#include "cPlayer.h"

class EntityHandler
{
public:
	DynamicArray<Entity*> entities;

	DynamicArray<Player*> players;

	EntityHandler()
	{
		entities.allocate(2048);
	}

	template<class T>
	T* createEntity(u32 id)
	{
		T* entity = new T();
		entities.pushBack(entity);

		if (entity->isPlayer())
			players.pushBack(entity);

		entity->id = id;

		return entity;
	}

	void deleteEntity(u32 id)
	{
		for (Entity* ent : entities)
			if (ent->id == id)
				ent->toDelete = true;
	}

	template<class T>
	T* getEntity(u32 id)
	{
		for (Entity* ent : entities)
			if (ent->id == id)
				return (T*)ent;
		return nullptr;
	}

	void update()
	{
		for (int i = (int)entities.count - 1; i >= 0; i--)
		{
			Entity* entity = entities[i];

			if (entity->remove)
			{
				if (entity->isPlayer())
					players.fastRemove(i);

				delete(entity);
				entities.fastRemove(i);
				continue;
			}

			if (entity->toDelete)
			{
				entity->remove = true;
				continue;
			}

			entity->update();
		}
	}
};