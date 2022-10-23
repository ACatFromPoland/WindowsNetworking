#include "cPacketHandler.h"

void handlePacket(packetData& packet, World& world)
{
	packet.buffer.clear();

	StarterData starterData = packet.read<StarterData>();
	for (int i = 0; i < starterData.headerCount; i++)
	{
		handleHeader(packet, world);
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

void handleEntities(packetData& packet, World& world)
{
	EntityData entData = packet.read<EntityData>();

	for (i32 i = 0; i < entData.count; i++)
	{
		EntityID entity = packet.read<EntityID>();
		switch (entity.type)
		{
			case EntityTypes::ENT_PLAYER:
			{
				if (entity.remove)
				{
					world.entityHandler.deleteEntity(entity.id);
					break;
				}

				Player* player = world.entityHandler.getEntity<Player>(entity.id);
				if (!player)
				{
					player = world.entityHandler.createEntity<Player>(entity.id);
					break;
				}

				player->readFromBuffer(packet.buffer);
				break;
			}
		}
	}
}
