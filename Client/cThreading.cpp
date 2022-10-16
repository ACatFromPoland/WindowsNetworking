#include "cThreading.h"

#include <iostream>

void packetThread(NetClient* net, DynamicArray<packetData>* queue)
{
	NetBuffer recvBuffer;

	while (true)
	{
		memset(recvBuffer.buffer, 0, NetBuffer::size);
		if (!net->Recv(recvBuffer.buffer, NetBuffer::size))
			continue;

		//TODO: Clean this up!
		net->threadLock.lock();
		if (queue->count > 2)
		{
			packetData& p = queue->back();
			memcpy(p.buffer.buffer, recvBuffer.buffer, NetBuffer::size);
			p.buffer.clear();
		}
		else
		{
			packetData& p = queue->pushBack();
			memcpy(p.buffer.buffer, recvBuffer.buffer, NetBuffer::size);
			p.buffer.clear();
		}
		net->threadLock.unlock();
	
		recvBuffer.clear();
	}
}