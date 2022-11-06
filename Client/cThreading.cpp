#include "cThreading.h"

#include <iostream>

void packetThread(NetClient* net, DynamicArray<packetData>* queue)
{
	NetBuffer recvBuffer;

	while (true)
	{
		if (!net->Recv(recvBuffer.buffer, NetBuffer::size))
			continue;

		//TODO: Clean this up!
		net->threadLock.lock();
		packetData& p = queue->pushBack();
		p.buffer = recvBuffer;
		p.buffer.clear();
		net->threadLock.unlock();
	
		recvBuffer.clear();
	}
}