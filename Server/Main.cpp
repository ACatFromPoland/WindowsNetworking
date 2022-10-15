#include "Server.h"

int main()
{
	Server net;
	net.Setup();

	NetClock clock(66.0f);
	Enviroment World;

	if (!net.Begin(5006))
		net.WSAError("Error starting Server!");

	std::thread netThread(serverThread, &net);

	while (true)
	{	 
		net.ThreadLock.lock();
		for (PacketData& p : g_packetQueue)
			handlePacket(p, net, World);
		g_packetQueue.purge();
		net.ThreadLock.unlock();

		// Update
		World.Update();

		if (g_netSendBuffer.isEmpty())
			BUFFER_DATA(g_netSendBuffer, (int)0);

		BUFFER_HEADER(g_netSendBuffer, HeaderType::ENTITY);
		BUFFER_DATA(g_netSendBuffer, EntityTypes::PLAYER);
		BUFFER_DATA(g_netSendBuffer, (unsigned char)World.players.count);

		for (Player* p : World.players)
		{
			p->WriteToBuffer(g_netSendBuffer);
		}

		for (Player* player : World.players)
		{
			net.SendTo(g_netSendBuffer.buffer, NetBuffer::size, player->client);
		}
		g_netSendBuffer.clear();
	}

	return 0;
}