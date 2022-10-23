#include "NetObject.h"
#include "NetVar.h"

NetVarBase* getNetVar(void* thisptr, ptrdiff_t offset)
{
	//printf("NETVAR: %s\n", typeid(this).name());
	return (NetVarBase*)((ptrdiff_t)thisptr + offset);
}

void NetObject::writeToBuffer(NetBuffer& buffer)
{
	for (ptrdiff_t var : netVars)
	{
		NetVarBase* p = getNetVar(this, var);
		p->write(buffer);
	}
}

void NetObject::readFromBuffer(NetBuffer& buffer)
{
	for (ptrdiff_t var : netVars)
	{
		getNetVar(this, var)->read(buffer);
	}
}

// Dead Code
//int NetObject::getNetVarSize()
//{
//	int size = 0;
//	for (ptrdiff_t var : netVars)
//	{
//		size += getNetVar(var)->getSize();
//	}
//	return size;
//}