#include "ServerNet.h"
#include <time.h>
int main()
{
	ANetwork::Setup();
	ANetwork::Bind(5006);

	NetClock clock(66.0f);

	std::thread networkThread(NetworkThread);

	// Setup Server
	// Branch test
	while (true)
	{
		// TODO: Update all clients with player positions
		// Handle Server Updates
		if (clock.Tick())
		{
			WorldPacket data{};
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

		// Handle Client Packet
		ANetwork::threadLock.lock();
		for (QueueElement& q : packetQueue)
			HandleMessage(q.msg, q.address);
		packetQueue.purge();
		ANetwork::threadLock.unlock();
	}

	ANetwork::Close();
	return 0;
}