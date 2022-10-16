#include "SFML/Graphics.hpp"

#include <vector>
#include <iostream>

#include "ANET/Client.h"
#include "ANET/NetBuffer.h"

#include "Shared/NetShared.h"
#include "Shared/NetClock.h"
#include "Shared/DynamicArray.h"

#include "Shared/NetPlayer.h"

#include "cThreading.h"

static short localPlayerID = -1;

static int serverLastPacketID = 0;
static int clientLastPacketID = 0;
static int packetsDropped = 0;

class Player : public NetPlayer
{
public:
	Player(int _id, float _x, float _y)
	{
		id = _id;
		x = _x;
		y = _y;
	}

	void move(NetPlayer& ref)
	{
		x = ref.x;
		y = ref.y;
	}
};

struct World
{
	NetClient& client;
	std::vector<Player>& playerList;
	NetClock& clock;

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
};

void handlePacket(packetData& packet, World& world)
{
	/*int* n = (int*)packet.buffer.buffer;
	std::cout << n[1] << std::endl;*/

	i32 packetID = packet.read<i32>();
	if (packetID <= serverLastPacketID)
	{
		packetsDropped++;
		return;
	}

	i32 header = packet.read<i32>();
	if (header == TYPE_UPDATE)
	{
		i32 playerSize = packet.read<i32>();
		if (playerSize > 128 || playerSize < 0)
		{
			std::cout << "Mo Fucker tried to create " << playerSize << " players..." << std::endl;
			packetsDropped++;
			return;
		}

		for (i32 i = 0; i < playerSize; i++)
		{
			NetPlayer recvPlayer = packet.read<NetPlayer>();
			Player* player = world.getPlayer(recvPlayer.id);
			if (!player)
			{
				std::cout << "[PLAYER CREATED] " << recvPlayer.id << std::endl;
				world.playerList.push_back(Player(recvPlayer.id, recvPlayer.x, recvPlayer.y));
			}
			else
			{
				player->move(recvPlayer);
			}
		}

		if (world.playerList.size() != playerSize)
		{
			std::cout << "[ERROR] Expected Player Count: " << playerSize << "  Unique Player ID's in Packet: " << world.playerList.size() << std::endl;
			world.playerList.clear(); // Reset players
		}
	}
	else
		packetsDropped++;
}

int main()
{
	NetClient net;
	net.Setup();

	std::cout << "Enter IP!\n";
	std::string ip;
	std::cin >> ip;

	net.Begin(ip.c_str(), 5006);

	// Attempt Connection!
	{
		NetBuffer toSend;
		toSend.write<i32>(0);
		toSend.write<i32>(TYPE_CONNECT);
		net.Send(toSend.buffer, NetBuffer::size);
		
		NetBuffer toRecv;
		while (true)
		{
			net.Recv(toRecv.buffer, NetBuffer::size);

			i32 packetID = toRecv.read<i32>();
			if (packetID == 0)
			{
				i32 header = toRecv.read<i32>();
				if (header == TYPE_CONNECT)
					break;
			}
			toRecv.clear();
		}
		localPlayerID = toRecv.read<i32>();
		std::cout << "[Connected] Our ID: " << localPlayerID << std::endl;
	}

	// Connection Established, Start Game
	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);

	sf::RenderWindow window(sf::VideoMode(800, 800), "Trojan.exe");

	std::vector<Player> players;

	NetClock clock(66.0f);
	NetClock debugClock(0.5f);

	World world = { net, players, clock };

	bool focused = true;
	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::GainedFocus)
				focused = true;
			else if (event.type == sf::Event::LostFocus)
				focused = false;
		}

		net.threadLock.lock();
		for (packetData& packet : packetQueue)
			handlePacket(packet, world);
		packetQueue.clear();
		net.threadLock.unlock();
		
		if (debugClock.Tick()) 
		{
			std::cout << "\n";
			std::cout << "[Player Count] " << players.size() << std::endl;
			std::cout << "[Packets Dropped] " << packetsDropped << std::endl;
		}

		if (clock.Tick() && focused)
		{
			NetBuffer toSend;

			toSend.write<i32>(++clientLastPacketID);

			toSend.write<i32>(TYPE_MOVE);
			toSend.write<i32>(localPlayerID);

			bool inputs[4];
			inputs[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			inputs[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			inputs[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			inputs[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

			for (int i = 0; i < 4; i++)
			{
				bool temp = inputs[i];
				toSend.write<bool>(temp);
			}

			net.Send(toSend.buffer, NetBuffer::size);
		}

		// Render
		window.clear(sf::Color(146, 146, 144));

		sf::CircleShape circle;
		circle.setFillColor(sf::Color::Red);
		circle.setRadius(15.0f);

		for (Player& player : players)
		{
			circle.setPosition(player.x, player.y);
			window.draw(circle);
		}

		window.display();
	}

	return 0;
}