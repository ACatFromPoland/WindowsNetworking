#pragma once
#include "Network.h"
#include "Player.h"

void Client_Connect(Message& msg, sockaddr_in& address)
{
	// Recieve 
	ConnectPacket* connect = msg.fetch<ConnectPacket>();

	char ip[16];
	ANetwork::GetIPAddress(address, ip);

	printf("[CLIENT CONNECTED] NAME: %s  IP: %s:%d\n", connect->name, ip, (int) address.sin_port);

	// Respond
	Message toSend;
	FormatMessageData(toSend);

	ResponsePacket response = { 1 };
	AddPacket<ResponsePacket>(toSend, PacketType::ECHO_RESPOND, response);

	Send(toSend, address);

	// Player Stuff
	int playerId = addPlayer(address);

	// Send Init
	toSend;
	FormatMessageData(toSend);

	InitPacket init = { playerId, 12 };
	AddPacket<InitPacket>(toSend, PacketType::SERVER_DATA, init);

	Send(toSend, address);

	printf("[PLAYER CREATED] ID: %d\n", playerId);
}

void Client_Input(Message& msg, sockaddr_in& data)
{
	InputPacket* input = msg.fetch<InputPacket>();

	movePlayer(input->ID, input->input);
}

void Echo_Respond(Message& msg, sockaddr_in& data)
{
	ResponsePacket* starter = msg.fetch<ResponsePacket>();
}

typedef void (*HandlePacket)(Message&, sockaddr_in&);

HandlePacket packetHandlers[lastType] = {
	Client_Connect,
	nullptr,
	Client_Input,
	nullptr,
	Echo_Respond
};

void HandleMessage(Message& msg, sockaddr_in& data)
{
	msg.beginFetch();
	StarterPacket* starter = msg.fetch<StarterPacket>();
	// Verify packet isn't old...

	while ((size_t)msg.iter - (size_t)msg.memory < msg.count)
	{
		HeaderPacket* header = msg.fetch<HeaderPacket>();
		packetHandlers[header->type](msg, data);
	}
}