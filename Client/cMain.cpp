#include "SFML/Graphics.hpp"

#include "cThreading.h"
#include "cWorld.h"
#include "cPacketHandler.h"

#include "cPlayer.h"
#include "cRocket.h"

static short localPlayerID = -1;

int main()
{
	NetClient net;
	net.Setup();

	net.Begin("127.0.0.1", 5006);

	unsigned int classTypeInput = 0;
	std::cin >> classTypeInput;
	
	// Attempt Connection!
	{
		NetBuffer toSend;
		toSend.write<StarterData>({ 0, 1 });
		toSend.write<HeaderData>({HeaderTypes::HEADER_CONNECT, 1});
		toSend.write<ConnectData>({ (u8)classTypeInput });

		net.Send(toSend.buffer, NetBuffer::size);
		
		NetBuffer toRecv;
		net.Recv(toRecv.buffer, NetBuffer::size);

		StarterData starter = toRecv.read<StarterData>();
		HeaderData header = toRecv.read<HeaderData>();
		
		localPlayerID = toRecv.read<i32>();
	}

	// Connection Established, Start Game
	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);

	sf::RenderWindow window(sf::VideoMode(800, 800), "Trojan.exe");

	World world = World(&net);

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
		
		// Render

		world.entityHandler.update();
	
		window.clear(sf::Color(146, 146, 144));

		sf::CircleShape circle;

		sf::Color playerColors[9] =
		{
			sf::Color(178, 100, 90), // Scout
			sf::Color(32, 36, 39), // Soldier
			sf::Color(84, 47, 50), // Pyro
			sf::Color(76, 44, 32), // Demoman
			sf::Color(149, 123, 100), // Heavy
			sf::Color(217, 127, 66), // Engineer
			sf::Color(205, 180, 166), // Medic
			sf::Color(58, 56, 41), // Sniper
			sf::Color(93, 12, 32) // Spy
		};

		for (Entity* entity : world.entityHandler.entities)
		{
			if (!entity->isPlayer())
				continue;

			Player* p = (Player*)entity;
			circle.setPosition(sf::Vector2f(p->position.value.x, p->position.value.y));
			if (p->isBot)
				circle.setFillColor(sf::Color(78, 109, 123));
			else
				circle.setFillColor(playerColors[p->classType]);
			circle.setRadius(15.0f);
			window.draw(circle);
		}

		window.display();

		// Get inputs for next frame
		if (!world.clock.Tick())
			continue;

		NetBuffer toSend;
		StarterData& starter = toSend.write<StarterData>({ 0, 0 });
		
		toSend.write<HeaderData>({ HeaderTypes::HEADER_GENERIC, 1 });
		starter.headerCount++;

		toSend.write<i32>(localPlayerID);

		if (focused)
		{
			toSend.write<HeaderData>({ HeaderTypes::HEADER_MOVE, 1 });
			starter.headerCount++;
			
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
		}

		net.Send(toSend.buffer, NetBuffer::size);
	}

	return 0;
}