#include "NetObject.h"
#include "NetVar.h"

void NetObject::writeToBuffer(NetBuffer& buffer)
{
	for (ptrdiff_t var : netVars)
	{
		NetVarBase* p = getNetVar(var);
		p->write(buffer);
	}
}

void NetObject::readFromBuffer(NetBuffer& buffer)
{
	for (ptrdiff_t var : netVars)
	{
		getNetVar(var)->read(buffer);
	}
}

NetVarBase* NetObject::getNetVar(ptrdiff_t offset)
{
	return (NetVarBase*)((ptrdiff_t)this - 8 + offset);
}

// Dead Code
int NetObject::getNetVarSize()
{
	int size = 0;
	for (ptrdiff_t var : netVars)
	{
		size += getNetVar(var)->getSize();
	}
	return size;
}