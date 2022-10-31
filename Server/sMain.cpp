#include "ANET/Server.h"

#include "Shared/DynamicArray.h"
#include "Shared/NetPackets.h"
#include "Shared/NetObject.h"
#include "Shared/NetVar.h"
#include "Shared/NetClock.h"

#include "sThreading.h"

#include <cmath>

// TODO: Every entity should have AABB collider

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

struct colliderCircle
{
	Vector2 center;
	float radius;
};

struct colliderRay
{
	Vector2 origin;
	Vector2 direction;
	float t;
};

class NetEntityBase : public NetObject
{
public:
	static ENTITY_ID ID_ITERATOR;

	bool remove : 1;
	ENTITY_ID id : 15;
	EntityTypes type;
};
ENTITY_ID NetEntityBase::ID_ITERATOR = 0;

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
	colliderCircle circle;
	
	bitInt inputs;
	NetVector2(mousePosition);
	NetFloat(health);

	double timeSinceLastShot;

	Player();

	virtual void update(gameState& state);
};

Player::Player()
{
	type = EntityTypes::ENT_PLAYER;
	remove = false;
	health = 100.f;
	timeSinceLastShot = 0.0f;

	circle.radius = 15.0f;
	circle.center = position;
}

bool rayCast(colliderCircle& circle, colliderRay& ray)
{
	Vector2 e = circle.center - ray.origin;
	float rSq = circle.radius * circle.radius;

	float a = e.x * ray.direction.x + e.y * ray.direction.y;

	float eSq = (e.x * e.x + e.y * e.y);

	float bSq = eSq - (a * a);
	float f = std::sqrt(rSq - bSq);

	if (rSq - (eSq - (a * a)) < 0.0f)
	{
		ray.t = -1.0f;
		return false;
	}
	else if (eSq < rSq)
	{
		ray.t = a + f;
		return true;
	}
	else
	{
		ray.t = a - f;
		return true;
	}
}

void Player::update(gameState& state)
{
	if (health < 0.0f)
	{
		position = Vector2(400.0f, 400.0f);
		health = 100.0f;
	}
		
	if (inputs.get(IN_FORWARD)) position.value.y -= (float)(75.0 * state.deltaTime);
	if (inputs.get(IN_LEFT)) position.value.x -= (float)(75.0 * state.deltaTime);
	if (inputs.get(IN_BACK)) position.value.y += (float)(75.0 * state.deltaTime);
	if (inputs.get(IN_RIGHT)) position.value.x += (float)(75.0 * state.deltaTime);

	circle.center = position;

	if (inputs.get(IN_MOUSE1))
	{
		if (state.gameTime.timeSince(timeSinceLastShot) > 0.1)
		{
			colliderRay shot;
			shot.origin = position;
			shot.direction = (position - mousePosition).normalized();

			for (Entity* ent : state.entities)
			{
				if (ent->type != EntityTypes::ENT_PLAYER)
					continue;
				if (ent == this)
					continue;

				Player* player = (Player*)ent;
				colliderCircle& circle = player->circle;

				if (rayCast(circle, shot))
					player->health -= 12.0f;
			}

			timeSinceLastShot = state.gameTime.getTick();
		}
	}

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
	state.gameTime = NetClock(60.f);
	state.lastTick = state.gameTime.getTick();
}

void World::updateEntity(Entity* entity)
{
	entity->update(state);
}

struct client
{
	ENTITY_ID id;
	double lastPacketTick;
	sockaddr_in address;
};

int main()
{
	setConsoleMode();

	// Opens a server on port 5006
	NetServer net;
	net.Setup();
	net.Begin(5006);

	// Opens a listener thread for the server
	DynamicArray<packetData> packetQueue;
	std::thread recvThread(packetThread, &net, &packetQueue);

	// Create our connected clients
	DynamicArray<client> clients;

	World world;
	
	while (true)
	{
		net.threadLock.lock();
		for (packetData& packet : packetQueue)
		{

			StarterData starterData = packet.read<StarterData>();
			for (u32 i = 0; i < starterData.headerCount; i++)
			{
				HeaderData headerData = packet.read<HeaderData>();
				for (u32 j = 0; j < headerData.count; j++)
				{
					
					if (headerData.type == HeaderTypes::HEADER_GENERIC)
					{
						ActivityData activityData = packet.read<ActivityData>();
						for (client& c : clients)
						{
							if (activityData.id == c.id)
							{
								c.lastPacketTick = world.state.gameTime.getTick();
							}
						}
					}
					else if (headerData.type == HeaderTypes::HEADER_CONNECT)
					{
						ConnectData connectData = packet.read<ConnectData>();
						
						client& c = clients.pushBack();
						c.lastPacketTick = world.state.gameTime.getTick();
						c.address = packet.address;

						Player* player = new Player();
						player->classType = connectData.classType;

						c.id = player->id = Entity::ID_ITERATOR++;

						world.state.entities.pushBack(player);

						player->position = Vector2(400.0f, 400.0f);
						player->velocity = Vector2(0.0f, 0.0f);

						//
						NetBuffer toSend;
						u32& starterCount = toSend.write<StarterData>({ 0, 0 }).headerCount;
						
						u32& headerCount = toSend.write<HeaderData>({ HeaderTypes::HEADER_CONNECT, 0 }).count;
						starterCount++;

						toSend.write<InitData>({c.id});
						headerCount++;

						net.SendTo(toSend.buffer, NetBuffer::size, packet.address);
					}
					else if (headerData.type == HeaderTypes::HEADER_MOVE)
					{
						MoveData data = packet.read<MoveData>();
						
						for (Entity* entity : world.state.entities)
						{
							if (entity->id == data.id)
							{
								//TODO: check if entity is player
								if (entity->type != EntityTypes::ENT_PLAYER)
									break;

								Player* player = (Player*)entity;
								player->inputs = data.inputs;
								player->mousePosition = data.mousePosition;
							}
						}
					}
				}
			}
		}
		packetQueue.purge();
		net.threadLock.unlock();

		// Check for client connections
		for (int i = (int)(clients.count - 1); i >= 0; i--)
		{
			client& c = clients[i];

			if (world.state.gameTime.timeSince(c.lastPacketTick) > 5.0f)
			{
				for (Entity* entity : world.state.entities)
				{
					if (entity->id == c.id)
						entity->toDelete = true;
				}
			}
		}

		// Delta time
		world.state.deltaTime = world.state.gameTime.timeSince(world.state.lastTick);
		world.state.lastTick = world.state.gameTime.getTick();

		// Run frame on game state
		for (Entity* entity : world.state.entities)
		{
			/*if (entity->toDelete)
			{
				entity->remove = true;
				continue;
			}*/
			world.updateEntity(entity);
		}

		// Update clients on game state
		NetBuffer toSend;
		u32& starterCount = toSend.write<StarterData>({0, 0}).headerCount;
		
		u32& headerCount = toSend.write<HeaderData>({HeaderTypes::HEADER_ENTITY, 1}).count;
		starterCount++;

		u32& entCount = toSend.write<EntityData>({0}).count;
		for (Entity* entity : world.state.entities)
		{
			toSend.write<EntityID>({entity->remove, entity->id, entity->type});
			entity->writeToBuffer(toSend);
			entCount++;
		}

		for (client& c : clients)
		{
			net.SendTo(toSend.buffer, NetBuffer::size, c.address);
		}

		/*for (int i = (int)(clients.count - 1); i >= 0; i--)
		{
			Entity* entity = world.state.entities[i];
			if (entity->remove)
			{
				delete(entity);
				world.state.entities.fastRemove(i);
			}
		}*/
	}
	return 0;
}