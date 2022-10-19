#include <vector>
#include <iostream>

#include "ANET/Server.h"

#include "Shared/NetBuffer.h"
#include "Shared/NetShared.h"
#include "Shared/NetClock.h"
#include "Shared/DynamicArray.h"
#include "Shared/NetPackets.h"

#include "Shared/NetPlayer.h"
#include "Shared/NetRocket.h"

#include "sThreading.h"

struct World
{
	NetServer& server;
	DynamicArray<Player>& playerList;
	NetClock& clock;

	Player* clientExists(USHORT port) 
	{
		for (Player& player : playerList)
		{
			if (player.client.sin_port == port)
			{
				return &player;
			}
		}
		return nullptr;
	}

	Player* getPlayer(int id)
	{
		for (Player& player : playerList)
		{
			if (player.id == id)
			{
				return &player;
			}
		}
		return nullptr;
	}

	Player& createPlayer(sockaddr_in address)
	{
		int playerId = NetEntity::ID++;

		Player& newPlayer = playerList.pushBack();
		newPlayer.init(0, address, playerId, Vector2(400.0f, 400.0f));
		newPlayer.lastPacketTick = clock.getTick();

		NetBuffer connectPacket;
		connectPacket.write<StarterData>({0, 1});
		connectPacket.write<HeaderData>({HEADER_CONNECT, 1});
		
		connectPacket.write<i32>(playerId);

		server.SendTo(connectPacket.buffer, NetBuffer::size, address);
		return newPlayer;
	}
};

void handleHeader(packetData& packet, World& world)
{
	HeaderData headerData = packet.read<HeaderData>();
	for (int i = 0; i < headerData.count; i++)
	{
		switch (headerData.type)
		{	
			// TODO: Handle Default Case!
			case HeaderTypes::HEADER_GENERIC:
			{
				i32 id = packet.read<i32>();
				Player* player = world.getPlayer(id);
				if (!player)
					return;

				player->lastPacketTick = world.clock.getTick();
				break;
			}
			case HeaderTypes::HEADER_CONNECT:
			{
				Player& ref = world.createPlayer(packet.address);
				i32 playerId = ref.id;
				std::cout << "[PLAYER CREATED] " << playerId << std::endl;
				break;
			}
			case HeaderTypes::HEADER_MOVE:
			{
				MoveData movement = packet.read<MoveData>();
				Player* player = world.getPlayer(movement.id);
				if (!player)
					return;

				player->movePlayer(movement.inputs);
				
				if (movement.inputs.get(IN_MOUSE1))
				{
					sockaddr_in empty = {};
					Player& ref = world.createPlayer(empty);
					ref.position = movement.mousePosition;
					ref.bot = true;
				}
				break;
			}
			default:
				printf("Invalid Packet Header! %d\n", headerData.type);
		}
	}
}

void handlePacket(packetData& packet, World& world)
{
	StarterData starterData = packet.read<StarterData>();
	for (int i = 0; i < starterData.headerCount; i++)
	{
		handleHeader(packet, world);
	}
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

static int lastPacketID = 0;
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