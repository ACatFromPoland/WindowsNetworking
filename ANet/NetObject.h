#pragma once
class NetVarBase;

#include "Shared/DynamicArray.h"
#include "NetBuffer.h"

class NetObject
{
public:
	DynamicArray<NetVarBase*> netVariables;

	void WriteToBuffer(NetBuffer& buffer);
	void ReadFromBuffer(NetBuffer& buffer);
};

