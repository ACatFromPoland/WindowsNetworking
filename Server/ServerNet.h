#pragma once

struct PacketData
{
	NetBuffer buffer;
	sockaddr_in address;

	constexpr unsigned char* getBuffer()
	{
		return buffer.buffer;
	}
};

DynamicArray<PacketData> g_packetQueue;

void serverThread(Server* net)
{
	g_packetQueue.setSize(0);

	sockaddr_in address;
	while (true)
	{
		if (!net->RecvFrom(g_netRecvBuffer.buffer, NetBuffer::size, address))
		{
			net->WSAError("Error at recvfrom");
			break;
		}

		net->ThreadLock.lock();
		
		PacketData& pp = g_packetQueue.pushBack();
		pp.buffer.clear();
		memcpy(pp.buffer.buffer, g_netRecvBuffer.buffer, NetBuffer::size);
		pp.address = address;
		
		net->ThreadLock.unlock();

		g_netRecvBuffer.clear();
	}
}