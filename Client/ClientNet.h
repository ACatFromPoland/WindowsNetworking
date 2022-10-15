#pragma once
#include "Shared/DynamicArray.h"

DynamicArray<NetBuffer> g_packetQueue;

void clientThread(Client* net)
{
	g_packetQueue.setSize(0);

	NetBuffer temp;
	while (true)
	{
		if (!net->Recv(temp.buffer, NetBuffer::size))
		{
			net->WSAError("Error at recvfrom");
			break;
		}

		net->ThreadLock.lock();

		NetBuffer& packet = g_packetQueue.pushBack();
		memcpy(packet.buffer, temp.buffer, NetBuffer::size);

		net->ThreadLock.unlock();

		temp.clear();
	}
}