#include "ANET/Server.h"

#include "Shared/DynamicArray.h"
#include "Shared/NetPackets.h"
#include "Shared/NetObject.h"
#include "Shared/NetVar.h"
#include "Shared/NetClock.h"

#include "Shared/NetEntityBase.h"

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

struct colliderAABB
{
	Vector2 center;
	Vector2 size;

	Vector2 getMin()
	{
		Vector2 p1 = center + size;
		Vector2 p2 = center - size;
		return Vector2(fminf(p1.x, p2.x),
					   fminf(p1.y, p2.y));
	}

	Vector2 getMax()
	{
		Vector2 p1 = center + size;
		Vector2 p2 = center - size;
		return Vector2(fmaxf(p1.x, p2.x),
					   fmaxf(p1.y, p2.y));
	}
};

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

bool rayCast(colliderAABB& box, colliderRay& ray)
{
	Vector2 min = box.getMin();
	Vector2 max = box.getMax();

	float t1 = (min.x - ray.origin.x) / ray.direction.x;
	float t2 = (max.x - ray.origin.x) / ray.direction.x;
	float t3 = (min.y - ray.origin.y) / ray.direction.y;
	float t4 = (max.y - ray.origin.y) / ray.direction.y;

	float tmin = fmaxf(
		fminf(t1, t2),
		fminf(t3, t4)
	);

	float tmax = fminf(
		fmaxf(t1, t2),
		fmaxf(t3, t4)
	);

	if (tmax < 0) {
		ray.t = -1.0f;
		return false;
	}

	if (tmin > tmax) {
		ray.t = -1.0f;
		return false;
	}

	if (tmin < 0.0f) {
		ray.t = tmax;
		return true;
	}

	ray.t = tmin;
	return true;
}


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

	colliderAABB boundingBox;
	
	Entity();
	virtual void update(gameState& state) = 0;
};
Entity::Entity()
{
	type = EntityTypes::ENT_EMPTY;
	remove = false;
}

template <class Class>
Class* createEntity(gameState& state)
{
	Class* entity = new Class();
	entity->id = Entity::ID_ITERATOR++;

	state.entities.pushBack((Entity*)entity);
	return entity;
}

Entity* getEntity(gameState& state, ENTITY_ID id)
{
	for (Entity* entity : state.entities)
	{
		if (entity->id == id)
			return entity;
	}

	return nullptr;
}

class Player : public Entity
{
public:
	NetByte(classType);
	NetVector2(mousePosition);
	NetFloat(health);

	colliderCircle circle;
	bitInt inputs;
	double timeSinceLastShot;

	Player();
	virtual void update(gameState& state);
};

class Rocket : public Entity
{
public:
	Vector2 explodeAt;
	Rocket();
	virtual void update(gameState& state);
};

Rocket::Rocket()
{
	type = EntityTypes::ENT_ROCKET;
}

void Rocket::update(gameState& state)
{
	bool outOfBounds = position.value.x > 1000.0f ||
		position.value.x < 0.0f ||
		position.value.y > 1000.0f ||
		position.value.y < 0.0f;

	if (outOfBounds)
		toDelete = true;

	if (position.value.dist(explodeAt) < 1.0f)
		toDelete = true; // EXPLODE!

	if (toDelete)
	{
		for (Entity* entity : state.entities)
		{
			if (entity->type == EntityTypes::ENT_PLAYER)
			{
				Player* player = (Player*)entity;

				float dist = (float)player->position.value.dist(position.value);

				if (dist < 40.0f)
				{
					player->health -= ((dist - 40.0f) * (dist - 40.0f)) * 0.05f;
				}
			}
		}
	}

	position += velocity.value * (float)state.deltaTime;
	boundingBox.center = position; // Fix
}


Player::Player()
{
	type = EntityTypes::ENT_PLAYER;
	health = 100.f;
	timeSinceLastShot = 0.0f;

	circle.radius = 15.0f;
	circle.center = position;

	boundingBox.center = Vector2(circle.center.x, circle.center.y);
	boundingBox.size = Vector2(circle.radius, circle.radius);
}

void Player::update(gameState& state)
{
	if (health < 0.0f)
	{
		position = Vector2(400.0f, 400.0f);
		health = 100.0f;
	}
		
	if (inputs.get(IN_FORWARD)) position.value.y -= (float)(90.0 * state.deltaTime);
	if (inputs.get(IN_LEFT)) position.value.x -= (float)(90.0 * state.deltaTime);
	if (inputs.get(IN_BACK)) position.value.y += (float)(90.0 * state.deltaTime);
	if (inputs.get(IN_RIGHT)) position.value.x += (float)(90.0 * state.deltaTime);

	circle.center = position;
	boundingBox.center = Vector2(circle.center.x, circle.center.y);

	if (inputs.get(IN_MOUSE1))
	{
		if (classType == 0)
		{
			if (state.gameTime.timeSince(timeSinceLastShot) > 0.1)
			{
				colliderRay shot;
				shot.origin = position;

				shot.direction = position.value.directionTo(mousePosition.value);

				for (Entity* ent : state.entities)
				{
					if (ent == this)
						continue;

					if (rayCast(ent->boundingBox, shot))
					{
						if (ent->type != EntityTypes::ENT_PLAYER)
							break;

						Player* player = (Player*)ent;
						colliderCircle& circle = player->circle;

						if (rayCast(circle, shot))
							player->health -= 12.0f;
					}
				}
				timeSinceLastShot = state.gameTime.getTick();
			}
		}
		else if (classType == 1)
		{
			if (state.gameTime.timeSince(timeSinceLastShot) > 1.0)
			{
				Rocket* rocket = createEntity<Rocket>(state);
				rocket->position = position;
				rocket->velocity = position.value.directionTo(mousePosition.value) * 300.0f;
				rocket->explodeAt = mousePosition.value;

				timeSinceLastShot = state.gameTime.getTick();
			}
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
	state.gameTime = NetClock(60.0);
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
						
						// Create Player
						Player* player = createEntity<Player>(world.state);
						player->classType = connectData.classType;
						player->position = Vector2(50.0f, 50.0f);
						player->velocity = Vector2(0.0f, 0.0f);
						
						// Save connection
						client& c = clients.pushBack();
						c.id = player->id;
						c.lastPacketTick = world.state.gameTime.getTick();
						c.address = packet.address;

						// Send init
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

						Entity* entity = getEntity(world.state, data.id);

						if (entity->type == EntityTypes::ENT_PLAYER)
						{
							Player* player = (Player*)entity;
							player->inputs = data.inputs;
							player->mousePosition = data.mousePosition;
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
				Entity* entity = getEntity(world.state, c.id);
				entity->toDelete = true;
				clients.fastRemove(i);
			}
		}

		// Delta time
		world.state.deltaTime = world.state.gameTime.timeSince(world.state.lastTick);
		world.state.lastTick = world.state.gameTime.getTick();

		// Run frame on game state
		for (Entity* entity : world.state.entities)
		{
			if (entity->toDelete)
			{
				entity->remove = true;
				continue;
			}
			world.updateEntity(entity);
		}

		// Update clients on game state
		//if (!world.state.gameTime.Tick())
			//continue;
		
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

		for (int i = (int)(world.state.entities.count - 1); i >= 0; i--)
		{
			Entity* entity = world.state.entities[i];
			if (entity->remove)
			{
				delete(entity);
				world.state.entities.fastRemove(i);
			}
		}
	}
	return 0;
}