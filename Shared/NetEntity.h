#pragma once
#include "NetVar.h"
#include "NetShared.h"

class NetEntity : public NetObject
{
public:
	static int ID;
	i32 id;
	
	virtual void update() = 0;
};