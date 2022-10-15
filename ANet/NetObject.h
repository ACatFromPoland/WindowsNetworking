#pragma once
class NetVarBase;

#include "Shared/DynamicArray.h"
#include "NetBuffer.h"

class NetObject
{
public:
	// I don't think Dynami Array likes pointers
	DynamicArray<NetVarBase*> netVariables;

	void WriteToBuffer(NetBuffer& buffer);
	void ReadFromBuffer(NetBuffer& buffer);
};