#pragma once
#include "Shared/Vector2.h"
#include "ANet/NetVar.h"

class NetEntity : public NetObject
{
public:
	NetEntType(type);
	NetInt(id);
	NetVector2(position);
	NetVector2(velocity);

	virtual void _update() = 0;
};