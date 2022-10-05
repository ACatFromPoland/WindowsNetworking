#pragma once
#include "Network.h"
#include "CPlayer.h"
#include "SFML/Graphics.hpp"

void NetThreadError(sockaddr_in& address)
{
	// Figure Stuff Out :(
	// TODO: 
}

// TODO: Handle Given Player Count
void Server_Data(Message& msg)
{
	InitPacket* init = msg.fetch<InitPacket>();
	LocalClient.id = init->ID;
	players[init->ID].active = true;
}

// TODO: Update All Players With Sf::Vector2f
void Server_World(Message& msg)
{
	WorldPacket* p = msg.fetch<WorldPacket>();
	for (int i = 0; i < maxPlayerCount; i++)
	{
		players[i].active = p->active[i];
		players[i].x = p->pos[2 * i];
		players[i].y = p->pos[2 * i + 1];
	}
}

// TODO: Verify Last Packet
void Echo_Respond(Message& msg)
{
	ResponsePacket* starter = msg.fetch<ResponsePacket>();
}


typedef void (*HandlePacket)(Message&);

HandlePacket packetHandlers[lastType] = {
	nullptr,
	Server_Data,
	nullptr,
	Server_World,
	Echo_Respond
};

void HandleMessage(Message& msg)
{
	msg.beginFetch();
	StarterPacket* starter = msg.fetch<StarterPacket>();

	while ((size_t)msg.iter - (size_t)msg.memory < msg.count)
	{
		HeaderPacket* header = msg.fetch<HeaderPacket>();
		packetHandlers[header->type](msg);
	}
}