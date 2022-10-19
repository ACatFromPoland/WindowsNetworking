#pragma once
#include "Shared/DynamicArray.h"
#include "Shared/NetBuffer.h"
#include "ANET/Client.h"

struct packetData
{
	NetBuffer buffer;

	template <class T>
	T read()
	{
		return buffer.read<T>();
	}
};

void packetThread(NetClient* net, DynamicArray<packetData>* queue);