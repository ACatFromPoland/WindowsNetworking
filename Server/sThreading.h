#pragma once
//#include <vector>
#include "Shared/DynamicArray.h"
#include "Shared/NetBuffer.h"
#include "ANET/Server.h"

struct packetData
{
	NetBuffer buffer;
	sockaddr_in address;
	
	template <class T>
	T read()
	{
		return buffer.read<T>();
	}

	USHORT clientPort() { return address.sin_port; }
};

void packetThread(NetServer* net, DynamicArray<packetData>* queue);