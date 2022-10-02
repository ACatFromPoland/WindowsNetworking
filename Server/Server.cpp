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
		// Handle Server Updates
		WorldPacket data;
		for (int i = 0; i < maxPlayers; i++)
		{
			data.ids[i] = Player::players[i].id;
			if (data.ids[i] == invalidID)
				continue;

			data.pos[2 * i] = Player::players[i].x;
			data.pos[2 * i + 1] = Player::players[i].y;
		}

		for (int i = 0; i < maxPlayers; i++)
		{
			Player& p = Player::players[i];

			if (p.id == invalidID)
				continue;

			Message toSend;
			FormatMessageData(toSend);

			AddPacket<WorldPacket>(toSend, PacketType::SERVER_WORLD, data);

			Send(toSend, p.client);
		}
		// Handle Client Packet
	
		if (clock.Tick())
		{
			ANetwork::threadLock.lock();
			if (ready)
			{
				HandleMessage(msgBuffer, addrBuffer);
				ready = false;
			}
			/*
			for (int i = packetQueue.count - 1; i >= 0; i--)
			{
				QueueElement& q = packetQueue[i];
				HandleMessage(q.msg, q.address);
				packetQueue.popBack();
			}
			*/
			ANetwork::threadLock.unlock();
		}
	}

	ANetwork::Close();
	return 0;
}