#include "sPacketHandler.h"
#include "sThreading.h"
#include "sWorld.h"

void setConsoleMode();

int main()
{
	setConsoleMode();

	DynamicArray<Player> players;

	players.allocate(NetPlayer::maxPlayers);

	NetServer net;
	net.Setup();
	net.Begin(5006);

	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);
	
	NetClock clock(66.0f);
	NetClock debugClock(0.05f);

	// Keeps everything together
	World world { net, players, clock };

	while (true)
	{
		// Handle new packets
		net.threadLock.lock();
		for (packetData& packet : packetQueue)
			handlePacket(packet, world);
		packetQueue.purge();
		net.threadLock.unlock();

		if (debugClock.Tick())
		{
			std::cout << "[Player Count] " << players.count << std::endl;
		}

		if (!clock.Tick())
			continue;

		for (int i = (int)players.count - 1; i >= 0; i--)
		{
			Player& player = players[i];
			if (player.bot)
				continue;
			if (clock.timeSince(player.lastPacketTick) < 1.0f)
				continue;

			if (player.id < 0)
			{
				std::cout << "Player Disconnected!" << std::endl;
				players.fastRemove(i);
			}
			else
			{
				player.id = -player.id;
			}
		}

		// Update Players
		if (!players.isEmpty())
		{
			NetBuffer toSend;
			toSend.write<StarterData>({++lastPacketID, 1});
			toSend.write<HeaderData>({HeaderTypes::HEADER_ENTITY, 1});
			
			toSend.write<EntityData>({EntityTypes::ENT_PLAYER, (i32)players.count});
			for (Player& ref : players)
			{
				toSend.write<EntityID>({ref.id});
				ref.writeToBuffer(toSend);
			}

			for (Player& player : players)
				if (!player.bot)
					net.SendTo(toSend.buffer, NetBuffer::size, player.client);
		}
	}

	return 0;
}

void setConsoleMode()
{
	HANDLE conHandle = GetStdHandle(-10);
	DWORD mode;
	if (!GetConsoleMode(conHandle, &mode))
		printf("No Console?\n");
	mode = mode & ~(64 | 128); // Disable QuickEditMode
	if (!SetConsoleMode(conHandle, mode))
		printf("Can't Set Console Mode!\n");
}