#include "SFML/Graphics.hpp"

#include <vector>
#include <iostream>

#include "ANET/Client.h"
#include "Shared/NetBuffer.h"

#include "Shared/NetShared.h"
#include "Shared/NetClock.h"
#include "Shared/DynamicArray.h"
#include "Shared/NetPlayer.h"
#include "Shared/NetPackets.h"

#include "cThreading.h"

static short localPlayerID = -1;

static int serverLastPacketID = 0;
static int clientLastPacketID = 0;
static int packetsDropped = 0;

class Player : public NetPlayer
{
public:
	Player()
	{

	}

	virtual void update()
	{

	}

	virtual void foo()
	{

	}

	void move(NetPlayer& ref)
	{
		position = ref.position;
	}
};

struct World
{
	NetClient& client;
	DynamicArray<Player>& playerList;
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

void handleEntities(packetData& packet, World& world)
{
	EntityData entData = packet.read<EntityData>();

	for (i32 i = 0; i < entData.count; i++)
	{
		EntityID entity = packet.read<EntityID>();
		switch (entData.type)
		{
			case EntityTypes::ENT_PLAYER:
			{
				if (entity.id < 0)
				{
					std::cout << "Player Disconnected!" << std::endl;
					i32 temp = abs(entity.id);
					for (int i = 0; i < world.playerList.count; i++)
					{
						if (world.playerList[i].id == temp)
							world.playerList.fastRemove(i);
					}
					break;
				}

				Player* player = world.getPlayer(entity.id);
				if (!player)
				{
					std::cout << "[PLAYER CREATED] " << entity.id << std::endl;
					Player& player = world.playerList.pushBack();
					player.id = entity.id;
					player.readFromBuffer(packet.buffer);
				}
				else
				{
					player->readFromBuffer(packet.buffer);
				}
				break;
			}
		}
	}
}

void handleHeader(packetData& packet, World& world)
{
	HeaderData headerData = packet.read<HeaderData>();
	for (int i = 0; i < headerData.count; i++)
	{
		switch (headerData.type)
		{
			case HeaderTypes::HEADER_ENTITY:
			{
				handleEntities(packet, world);
				break;
			}
		}
	}
}

void handlePacket(packetData& packet, World& world)
{
	packet.buffer.clear();
	
	StarterData starterData = packet.read<StarterData>();
	for (int i = 0; i < starterData.headerCount; i++)
	{
		handleHeader(packet, world);
	}
}

int main()
{
	NetClient net;
	net.Setup();
	
	/*printf("IP...\n");
	std::string ip;
	std::cin >> ip;*/
	net.Begin("127.0.0.1", 5006);
	//net.Begin(ip.c_str(), 5006);

	// Attempt Connection!
	{
		NetBuffer toSend;
		toSend.write<StarterData>({ 0, 1 });
		toSend.write<HeaderData>({HeaderTypes::HEADER_CONNECT, 1});
		net.Send(toSend.buffer, NetBuffer::size);
		
		NetBuffer toRecv;
		net.Recv(toRecv.buffer, NetBuffer::size);

		StarterData starter = toRecv.read<StarterData>();
		HeaderData header = toRecv.read<HeaderData>();
		
		localPlayerID = toRecv.read<i32>();
		
		std::cout << "[Connected] Our ID: " << localPlayerID << std::endl;
	}

	// Connection Established, Start Game
	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);

	sf::RenderWindow window(sf::VideoMode(800, 800), "Trojan.exe");

	DynamicArray<Player> players;
	players.allocate(NetPlayer::maxPlayers);

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
			std::cout << "[Player Count] " << players.count << std::endl;
			std::cout << "[Packets Dropped] " << packetsDropped << std::endl;
		}

		if (clock.Tick())
		{
			NetBuffer toSend;
			toSend.write<StarterData>({0, 0});
			toSend.write<HeaderData>({ HeaderTypes::HEADER_GENERIC, 1 });
			toSend.write<i32>(localPlayerID);
			((StarterData*)&toSend.buffer)->headerCount++;

			if (focused)
			{
				toSend.write<HeaderData>({ HeaderTypes::HEADER_MOVE, 1 });

				MoveData data;

				data.id = localPlayerID;
				data.inputs.set(IN_FORWARD, sf::Keyboard::isKeyPressed(sf::Keyboard::W));
				data.inputs.set(IN_LEFT, sf::Keyboard::isKeyPressed(sf::Keyboard::A));
				data.inputs.set(IN_BACK, sf::Keyboard::isKeyPressed(sf::Keyboard::S));
				data.inputs.set(IN_RIGHT, sf::Keyboard::isKeyPressed(sf::Keyboard::D));
				data.inputs.set(IN_MOUSE1, sf::Mouse::isButtonPressed(sf::Mouse::Left));

				sf::Vector2f mousePos = sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				data.mousePosition = Vector2(mousePos.x, mousePos.y);

				toSend.write<MoveData>(data);

				((StarterData*)&toSend.buffer)->headerCount++;
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
			Vector2& pos = player.position.value;
			sf::Vector2f pPosition = sf::Vector2f(pos.x, pos.y);
			circle.setPosition(pPosition);
			window.draw(circle);
		}

		window.display();
	}

	return 0;
}