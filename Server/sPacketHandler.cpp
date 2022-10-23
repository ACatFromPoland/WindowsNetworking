#include "sPacketHandler.h"

void handlePacket(packetData& packet, World& world)
{
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
			// TODO: Handle Default Case!
		case HeaderTypes::HEADER_GENERIC:
		{
			u32 id = packet.read<u32>();
			Player* player = world.getPlayer(id);
			if (!player)
				return;

			player->lastPacketTick = world.clock.getTick();
			break;
		}
		case HeaderTypes::HEADER_CONNECT:
		{
			Player* player = world.createPlayer(packet.address);
			break;
		}
		case HeaderTypes::HEADER_MOVE:
		{
			MoveData movement = packet.read<MoveData>();
			Player* player = world.getPlayer(movement.id);
			if (!player)
				return;

			player->setInputs(movement.inputs, movement.mousePosition);
			break;
		}
		default:
			printf("Invalid Packet Header! %d\n", headerData.type);
		}
	}
}