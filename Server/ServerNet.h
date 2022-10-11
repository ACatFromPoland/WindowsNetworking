#pragma once
#include "ANet/Host.h"
#include "ANet/NetBuffer.h"
#include "Shared/NetworkTypes.h"
#include "Shared/DynamicArray.h"
#include "Enviroment.h"

struct PacketData
{
	NetBuffer buffer;
	sockaddr_in address;
};
DynamicArray<PacketData> toSendPackets;
DynamicArray<PacketData> toRecvPackets;

void clientInfoData(PacketData& data, Host& net, Enviroment& world)
{
	NetBuffer response;

	// How many headers
	response.write<int>(2);
	
	// Server Info Header
	response.write<unsigned char>(HeaderType::SERVER_INFO);
	
	int n = world.addPlayer();
	response.write<int>(n);
	response.write<unsigned char>(16);

	// Create Entity Header
	response.write<unsigned char>(HeaderType::CREATE_ENTITY);

	// Add All Entities
	response.write<int>(EntityTypes::ENTITY);
	response.write<unsigned char>((unsigned char)world.entities.count);

	net.SendTo(response.buffer, response.size, data.address);
}

void clientInputData(PacketData& data, Host& net, Enviroment& world)
{
	int pId = data.buffer.read<int>();
	bool inputs[Player::playerInputsSize];
	for (int i = 0; i < Player::playerInputsSize; i++)
	{
		inputs[i] = data.buffer.read<bool>();
	}

	world.movePlayer(pId, inputs, Player::playerInputsSize);
}

typedef void (*packetFunc)(PacketData&, Host& net, Enviroment&);
packetFunc packetHandlers[HeaderType::LAST_HEADERTYPE] = {
	nullptr,
	clientInfoData,
	clientInputData,
	nullptr,
};

void HandlePacket(PacketData& packet, Host& net, Enviroment& world)
{
	int headers = packet.buffer.read<int>();
	for (int i = 0; i < headers; i++)
	{
		HeaderType type = packet.buffer.read<HeaderType>();
		int count = packet.buffer.read<int>();
		for (int j = 0; j < count; j++)
			packetHandlers[type](packet, net, world);
	}
}

void serverThread(Host& Net)
{
	while (true)
	{
		// Recv Data
		sockaddr_in fromAddress;
		NetBuffer recvBuffer;
		if (!Net.RecvFrom(recvBuffer.buffer, NetBuffer::size, fromAddress))
		{
			Net.WSAError("Error at recvfrom");
			break;
		}

		Net.ThreadLock.lock();
		toRecvPackets.pushBack({ recvBuffer, fromAddress });
		Net.ThreadLock.unlock();
	}
}