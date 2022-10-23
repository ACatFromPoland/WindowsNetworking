#pragma once
#include "NetVar.h"
#include "NetShared.h"
#include "NetPackets.h"

class NetEntityBase
{
public:
	static u32 ID;

	bool remove : 1;
	u32 id : 31;
	EntityTypes type;

	virtual void update() = 0;
};