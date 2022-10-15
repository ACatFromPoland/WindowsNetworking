#pragma once

#include "ANet/Server.h"
#include "ANet/NetBuffer.h"
#include "Shared/Network/Network.h"

#include "Enviroment.h"
#include "ServerNet.h"

void handlePacket(PacketData& p, Server& net, Enviroment& world)
{
	int headerCount;
	BUFFER_READ(p.buffer, headerCount);

	for (int i = 0; i < headerCount; i++)
	{
		HeaderType header;
		BUFFER_READ(p.buffer, header);

		switch (header)
		{
			case GENERIC:
				break;

			case CONNECT:
			{
				// Read
				int classId;
				int weaponId;
				BUFFER_READ(p.buffer, classId);
				BUFFER_READ(p.buffer, weaponId);
				
				int n = world.addPlayer(p.address, classId, weaponId);

				// Buffer
				NetBuffer infoPacket;
				BUFFER_STARTER(infoPacket);
				BUFFER_HEADER(infoPacket, HeaderType::CONNECT);
				BUFFER_DATA(infoPacket, n);

				BUFFER_HEADER(infoPacket, HeaderType::ENTITY);
				BUFFER_DATA(infoPacket, EntityTypes::PLAYER);
				BUFFER_DATA(infoPacket, (unsigned char)world.players.count);
			
				for (Player* p : world.players)
				{
					p->WriteToBuffer(infoPacket);
				}

				if (!net.SendTo(infoPacket.buffer, NetBuffer::size, p.address))
					printf("ERROR FAILED TO SEND CONNECT PACKET!\n");

				break;
			}

			case MOVE:
			{
				int id;
				BUFFER_READ(p.buffer, id);
				InputStruct inputs;
				for (int i = 0; i < 6; i++)
				{
					inputs.inputs[i] = p.buffer.read<bool>();
				}
				world.movePlayer(id, inputs, 6);

				break;
			}
		}
	}
}