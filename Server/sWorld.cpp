#include "sWorld.h"
#include <iostream>

World::World(NetServer* network)
{
	server = network;
	clock = NetClock(66.0f);
}

Player* World::getPlayer(u32 id)
{
	for (Player* player : entityHandler.players)
	{
		if (player->id == id)
		{
			return player;
		}
	}
	return nullptr;
}

Player* World::createPlayer(sockaddr_in address)
{
	// Create Player
	Player* player = entityHandler.createEntity<Player>();
	player->client = address;
	player->position = Vector2(400.0f, 400.0f);
	player->lastPacketTick = clock.getTick();

	// Send init
	NetBuffer connectPacket;
	connectPacket.write<StarterData>({ 0, 1 });
	connectPacket.write<HeaderData>({ HEADER_CONNECT, 1 });

	connectPacket.write<u32>(player->id);

	server->SendTo(connectPacket.buffer, NetBuffer::size, address);
	return player;
}

void World::checkConnections()
{
	for (int i = (int)entityHandler.players.count - 1; i >= 0; i--)
	{
		Player* player = entityHandler.players[i];
		
		if (clock.timeSince(player->lastPacketTick) < 1.0f)
			continue;

		player->toDelete = true;
	}
}

void World::update()
{
	entityHandler.update();
}

void World::updateClients()
{
	if (!clock.Tick())
		return;

	NetBuffer snapshot;
	StarterData& starter = snapshot.write<StarterData>({0, 0});

	snapshot.write<HeaderData>({ HEADER_ENTITY, 1 });
	starter.headerCount++;

	snapshot.write<EntityData>({(i32)entityHandler.entities.count});
	for (Entity* ent : entityHandler.entities)
	{
		snapshot.write<EntityID>( {ent->remove, ent->id, ent->type} );
		ent->writeToBuffer(snapshot);
	}

	for (Player* player : entityHandler.players)
	{
		server->SendTo(snapshot.buffer, NetBuffer::size, player->client);
	}

	entityHandler.cleanDeletedEntities();
}