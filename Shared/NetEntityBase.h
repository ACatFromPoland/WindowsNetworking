#pragma once
#include "NetVar.h"
#include "NetShared.h"
#include "NetPackets.h"

class NetEntityBase
{
public:
	static ENTITY_ID ID;

	bool remove : 1;
	ENTITY_ID id : 15;
	EntityTypes type;

	virtual void update() = 0;
};