#pragma once
#include "NetObject.h"
#include "NetPackets.h"

class NetEntityBase : public NetObject
{
public:
	static ENTITY_ID ID_ITERATOR;

	bool remove : 1;
	ENTITY_ID id : 15;
	EntityTypes type;
};