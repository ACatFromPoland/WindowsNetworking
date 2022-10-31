#include "sThreading.h"

void packetThread(NetServer* net, DynamicArray<packetData>* queue)
{
	while (true)
	{	
		NetBuffer recvBuffer;
		sockaddr_in address;
		net->RecvFrom(recvBuffer.buffer, NetBuffer::size, address);

		net->threadLock.lock();
		packetData& p = queue->pushBack();
		p.buffer = recvBuffer;
		p.buffer.clear();
		p.address = address;

		net->threadLock.unlock();
	}
}
