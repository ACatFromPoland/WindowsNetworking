#include <vector>
#include <iostream>

#include "ANET/Server.h"
#include "ANET/NetBuffer.h"

#include "Shared/NetShared.h"
#include "Shared/NetClock.h"
#include "Shared/DynamicArray.h"
#include "Shared/NetPlayer.h"

#include "sThreading.h"

class Player : public NetPlayer
{
public:
	int lastPacketID;
	double lastPacketTick;
	static int ID;
	sockaddr_in client;

	Player(int _lastPacketID, sockaddr_in _client, int _id, float _x, float _y)
	{
		lastPacketTick = 0.0f;
		lastPacketID = _lastPacketID;
		client = _client;
		id = _id;
		x = _x;
		y = _y;
	}

	void movePlayer(bool inputs[4])
	{
		if (inputs[0]) y -= 1.0f;
		if (inputs[1]) y += 1.0f;
		if (inputs[2]) x -= 1.0f;
		if (inputs[3]) x += 1.0f;
	}
};
int Player::ID = 0;

struct World
{
	NetServer& server;
	std::vector<Player>& playerList;
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

	int createPlayer(sockaddr_in address)
	{
		int id = Player::ID++;
		playerList.push_back(Player(0, address, id, 400.0f, 400.0f));
		playerList.back().lastPacketTick = clock.getTick();

		NetBuffer init;
		init.write<i32>(TYPE_CONNECT);
		init.write<i32>(0);
		init.write<i32>(id);
		server.SendTo(init.buffer, NetBuffer::size, address);

		return id;
	}
};


void handlePacket(packetData& packet, World& world)
{
	// Check for stuff!
	i32 packetID = packet.read<i32>();
	Player* existingPlayer = world.clientExists(packet.clientPort());
	if (existingPlayer)
	{
		if (packetID > existingPlayer->lastPacketID)
			existingPlayer->lastPacketID = packetID;
		else
			return;
	}

	// Handle Packet
	i32 header = packet.read<i32>();
	if (header == TYPE_CONNECT)
	{
		int id = world.createPlayer(packet.address);
		std::string ip;
		std::cout << "[PLAYER CREATED] " << id << std::endl;
	}
	else if (header == TYPE_MOVE)
	{
		i32 playerID = packet.read<i32>();
		bool inputs[4];
		for (bool& b : inputs)
			b = packet.read<bool>();
		
		Player* player = world.getPlayer(playerID);
		if (!player)
			return;

		player->lastPacketTick = world.clock.getTick();
		player->movePlayer(inputs);
	}
}

static int lastPacketID = 0;
int main()
{
	std::vector<Player> players;

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
			std::cout << "[Player Count] " << players.size() << std::endl;
		}

		/*for (int i = (int)players.size() - 1; i >= 0; i--)
		{
			Player& player = players[i];
			if (clock.timeSince(player.lastPacketTick) > 5.0f)
			{
				std::cout << "Player Disconnected!" << std::endl;
				players.pop_back();
			}
		}*/

		if (!clock.Tick())
			continue;

		// Update Players
		if (!players.empty())
		{
			// TODO: Replace with NetVar writeToBuffer
			i8* playerEntityBuffer = (unsigned char*)malloc(sizeof(NetPlayer) * players.size());
			NetPlayer* iter = (NetPlayer*)playerEntityBuffer;
			for (int i = 0; i < players.size(); i++)
			{
				Player& ref = players[i];

				NetPlayer netPlayer{ ref.id, ref.x, ref.y };
				iter[i] = netPlayer;
			}

			NetBuffer toSend;
			toSend.write<i32>(++lastPacketID);

			toSend.write<i32>(TYPE_UPDATE);
			toSend.write<i32>((int)players.size());

			// Push player data array to send
			toSend.pushBack(playerEntityBuffer, sizeof(NetPlayer) * players.size());
			free(playerEntityBuffer);

			for (Player& player : players)
				net.SendTo(toSend.buffer, NetBuffer::size, player.client);
		}
	}

	return 0;
}