#pragma once
class NetVarBase;

#include "DynamicArray.h"
#include "NetBuffer.h"

class NetObject
{
public:
	DynamicArray<ptrdiff_t> netVars;

	virtual void writeToBuffer(NetBuffer& buffer);
	virtual void readFromBuffer(NetBuffer& buffer);
	//NetVarBase* getNetVar(ptrdiff_t offset);
	//int getNetVarSize();
};