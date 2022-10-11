#include "NetObject.h"
#include "NetVar.h"

void NetObject::WriteToBuffer(NetBuffer& buffer)
{
	for (NetVarBase* var : netVariables)
	{
		var->write(buffer);
	}
}

void NetObject::ReadFromBuffer(NetBuffer& buffer)
{
	for (NetVarBase* var : netVariables)
	{
		var->read(buffer);
	}
}