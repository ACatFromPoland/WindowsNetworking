#pragma once
#include "Shared/Vector2.h"
#include "Shared/Updateable.h"
#include "Shared/NetworkTypes.h"
#include "ANet/NetVar.h"

class Entity : public NetObject, public Updateable
{
public:
	NetEntType(type);
	NetInt(id);
	NetVector2(position);
	NetVector2(velocity);

	Entity();

	Entity(int _id, Vector2 _pos, Vector2 _vel);

	virtual void _update() = 0;
};