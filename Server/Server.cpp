#include "ServerNet.h"
#include <time.h>

int main()
{
	ANetwork::Setup();
	ANetwork::Bind(5006);

	NetClock clock(66.0f);
	std::thread networkThread(NetworkThread);

	// Setup Server

	while (true)
	{
		// TODO: Update all clients with player positions
		// Handle Server Updates
		if (clock.Tick())
		{
			WorldPacket data;
			for (int i = 0; i < maxPlayerCount; i++)
			{
				data.active[i] = players[i].active;
				data.pos[2 * i] = players[i].x;
				data.pos[2 * i + 1] = players[i].y;
			}

			for (Player& player : players)
			{
				if (!player.active)
					continue;

				Message toSend;
				FormatMessageData(toSend);

				AddPacket<WorldPacket>(toSend, PacketType::SERVER_WORLD, data);

				Send(toSend, player.client);
			}
		}
		
		/*
		Message toRecv;
		sockaddr_in client;

		if (Recv(toRecv, client))
			continue;

		QueueElement& newElm = packetQueue.pushBack();

		newElm.msg.Copy(element.msg);
		newElm.msg.bytes = element.msg.bytes;
		newElm.address = element.address;
		*/

		// Handle Client Packet
		ANetwork::threadLock.lock();
		for (int i = (int)counter - 1; i >= 0; i--)
		{
			QueueElement& q = packetQueue[i];
			HandleMessage(q.msg, q.address);

			counter--;
		}
		ANetwork::threadLock.unlock();
	}

	ANetwork::Close();
	return 0;
}