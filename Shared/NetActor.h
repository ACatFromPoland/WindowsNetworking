#pragma once
#include "NetObject.h"
#include "NetEntityBase.h"
#include <stdio.h>

class NetActor : public NetObject, public NetEntityBase
{
public:

	NetVector2(position);
	NetVector2(velocity);
};