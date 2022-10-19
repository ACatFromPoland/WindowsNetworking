#pragma once
class NetVarBase;

#include "DynamicArray.h"
#include "NetBuffer.h"

class NetObject
{
public:
	DynamicArray<ptrdiff_t> netVars;

	void writeToBuffer(NetBuffer& buffer);
	void readFromBuffer(NetBuffer& buffer);
	NetVarBase* getNetVar(ptrdiff_t offset);
	int getNetVarSize();
};