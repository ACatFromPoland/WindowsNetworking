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

		for (Entity* entity : world.entityHandler.entities)
		{
			if (!entity->isPlayer())
				continue;

			Player* p = (Player*)entity;

			circle.setPosition(sf::Vector2f(p->position.value.x, p->position.value.y));
			circle.setFillColor(sf::Color::Red);
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