#pragma once
#include "ANet/Server.h"
#include "Shared/NetClock.h"

#include "sPlayer.h"

class World
{
public:
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
		connectPacket.write<StarterData>({ 0, 1 });
		connectPacket.write<HeaderData>({ HEADER_CONNECT, 1 });

		connectPacket.write<i32>(playerId);

		server.SendTo(connectPacket.buffer, NetBuffer::size, address);
		return newPlayer;
	}
};