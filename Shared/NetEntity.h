#pragma once
#include "Shared/NetActor.h"

class Entity : public NetActor
{
public:
	bool toDelete;

	Entity()
	{
		toDelete = false;
	}

	virtual bool isPlayer() { return false; }
	virtual void update() {}
};