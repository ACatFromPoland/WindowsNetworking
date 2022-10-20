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
			i32 id = packet.read<i32>();
			Player* player = world.getPlayer(id);
			if (!player)
				return;

			player->lastPacketTick = world.clock.getTick();
			break;
		}
		case HeaderTypes::HEADER_CONNECT:
		{
			Player& ref = world.createPlayer(packet.address);
			i32 playerId = ref.id;
			std::cout << "[PLAYER CREATED] " << playerId << std::endl;
			break;
		}
		case HeaderTypes::HEADER_MOVE:
		{
			MoveData movement = packet.read<MoveData>();
			Player* player = world.getPlayer(movement.id);
			if (!player)
				return;

			player->movePlayer(movement.inputs);

			if (movement.inputs.get(IN_MOUSE1))
			{
				/*sockaddr_in empty = {};
				Player& ref = world.createPlayer(empty);
				ref.position = movement.mousePosition;
				ref.bot = true;*/
			}
			break;
		}
		default:
			printf("Invalid Packet Header! %d\n", headerData.type);
		}
	}
}
