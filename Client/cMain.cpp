#include "SFML/Graphics.hpp"
#include <iostream>

#include "cThreading.h"

#include "Shared/NetObject.h"
#include "Shared/NetVar.h"
#include "Shared/NetPackets.h"
#include "Shared/NetClock.h"
#include "Shared/NetEntityBase.h"

static ENTITY_ID localPlayerID = 0;

class Entity;
struct gameState
{
	double lastTick;
	double deltaTime;
	NetClock gameTime;
	DynamicArray<Entity*> entities;
};
class Entity : public NetEntityBase
{
public:
	bool toDelete = false;
	NetVector2(position);
	NetVector2(velocity);

	virtual void update(gameState& state) = 0;
};

class Player : public Entity
{
public:
	NetByte(classType);
	NetVector2(mousePosition);
	NetFloat(health);

	bool isBot;

	Player();
	virtual void update(gameState& state);
};

Player::Player()
{
	type = EntityTypes::ENT_PLAYER;
	isBot = false;
}

void Player::update(gameState& state)
{

}

class Rocket : public Entity
{
public:
	Rocket();
	virtual void update(gameState& state);
};

Rocket::Rocket()
{
	type = EntityTypes::ENT_ROCKET;
}

void Rocket::update(gameState& state)
{

}

class World
{
public:
	gameState state;

	World();
	void updateEntity(Entity* actor);
};

World::World()
{
	state.gameTime = NetClock(60.0);
	state.lastTick = state.gameTime.getTick();
}

void World::updateEntity(Entity* entity)
{
	entity->update(state);
}


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
		InitData init = toRecv.read<InitData>();

		localPlayerID = init.id;
	}

	// Connection Established, Start Game
	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);

	sf::RenderWindow window(sf::VideoMode(400, 400), "Trojan.exe");

	World world;

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
		{
			packet.buffer.clear();

			StarterData starterData = packet.read<StarterData>();
			for (u32 i = 0; i < starterData.headerCount; i++)
			{
				HeaderData headerData = packet.read<HeaderData>();

				for (u32 j = 0; j < headerData.count; j++)
				{
					if (headerData.type == HeaderTypes::HEADER_ENTITY)
					{
						EntityData entData = packet.read<EntityData>();

						for (u32 i = 0; i < entData.count; i++)
						{
							EntityID entityData = packet.read<EntityID>();
							if (entityData.remove)
							{
								for (int i = (int)world.state.entities.count - 1; i >= 0; i--)
								{
									Entity* ent = world.state.entities[i];
									if (entityData.id == ent->id)
									{
										delete(ent);
										world.state.entities.fastRemove(i);
										break;
									}
								}
								break;
							}
							
							Entity* entity = nullptr;
							for (Entity* search : world.state.entities)
							{
								if (search->id == entityData.id)
									entity = search;
							}

							if (!entity)
							{
								if (entityData.type == EntityTypes::ENT_PLAYER)
								{
									Player* player = new Player();
									player->id = entityData.id;
									world.state.entities.pushBack(player);
								}
								else if (entityData.type == EntityTypes::ENT_ROCKET)
								{
									Rocket* rocket = new Rocket();
									rocket->id = entityData.id;
									world.state.entities.pushBack(rocket);
								}
							}
							else
								entity->readFromBuffer(packet.buffer);
						}
					}
				}
			}
		}
		packetQueue.clear();
		net.threadLock.unlock();
		
		// Render

		for (int i = (int)world.state.entities.count - 1; i >= 0; i--)
		{
			Entity* ent = world.state.entities[i];

			if (ent->remove)
			{
				delete(ent);
				world.state.entities.fastRemove(i);
			}

			ent->update(world.state);
		}

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

		for (Entity* entity : world.state.entities)
		{
			if (entity->type == EntityTypes::ENT_PLAYER)
			{
				Player* p = (Player*)entity;

				// Draw body
				circle.setPosition(sf::Vector2f(p->position.value.x - 15.0f, p->position.value.y - 15.0f));
				if (p->isBot)
					circle.setFillColor(sf::Color(78, 109, 123));
				else
					circle.setFillColor(playerColors[p->classType]);
				circle.setRadius(15.0f);
				window.draw(circle);

				// Line
				Vector2 l = p->position.value.directionTo(p->mousePosition.value);
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(p->position.value.x, p->position.value.y), sf::Color::Red),
					sf::Vertex(sf::Vector2f(p->position.value.x + (l.x * 100.0f), p->position.value.y + (l.y * 100.0f)), sf::Color::Red)
				};
				window.draw(line, 2, sf::Lines);

				// Hands
				Vector2 looking = (Vector2(p->mousePosition.value.x, p->mousePosition.value.y) - p->position).normalized();
				circle.setFillColor((playerColors[(sf::Mouse::isButtonPressed(sf::Mouse::Left) ? (p->id == localPlayerID ? 5 : 1) : 1)]));
				circle.setRadius(4.0f);

				Vector2 handPos = (p->position - Vector2(4.0f, 4.0f));
				handPos += (looking * (15.0f + 4.0f));

				circle.setPosition(sf::Vector2f(handPos.x, handPos.y));

				window.draw(circle);

				// Health Bar
				sf::RectangleShape healthBar;
				healthBar.setPosition(sf::Vector2f(p->position.value.x - 25.0f, p->position.value.y - 30.0f));
				healthBar.setSize(sf::Vector2f(p->health / 2.0f, 4.f));
				healthBar.setFillColor(sf::Color::Green);
				window.draw(healthBar);
			}
			else if (entity->type == EntityTypes::ENT_ROCKET)
			{
				Rocket* r = (Rocket*)entity;
				circle.setPosition(sf::Vector2f(r->position.value.x - 5.0f, r->position.value.y - 5.0f));
				circle.setFillColor(sf::Color::Black);
				circle.setRadius(5.0f);
				window.draw(circle);
			}
		}
		
		window.display();

		// Get inputs for next frame
		if (!world.state.gameTime.Tick())
			continue;

		NetBuffer toSend;
		StarterData& starter = toSend.write<StarterData>({ 0, 0 });
		
		toSend.write<HeaderData>({ HeaderTypes::HEADER_GENERIC, 1 });
		starter.headerCount++;

		toSend.write<ActivityData>({ localPlayerID });
		starter.headerCount++;

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