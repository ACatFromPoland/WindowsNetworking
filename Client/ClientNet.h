#pragma once
#include "Network.h"
#include "SFML/Graphics.hpp"

const int maxPlayers = 8;
struct ClientData
{
	int id;
} client;

struct Player
{
	bool used;
	sf::Vector2f pos;
};

static Player players[maxPlayers];


// 
void Server_Data(Message& msg)
{
	InitPacket* init = msg.fetch<InitPacket>();
	client.id = init->ID;

	for (int i = 0; i < maxPlayers; i++)
	{
		players[i].used = false;
		players[i].pos = sf::Vector2f(0.0f, 0.0f);
	}

	players[client.id].used = true;
}

void Server_World(Message& msg)
{
	WorldPacket* p = msg.fetch<WorldPacket>();
	for (int i = 0; i < maxPlayers; i++)
	{
		if (p->ids[i] == -1)
			continue;

		players[i].pos = sf::Vector2f(p->pos[2 * i], p->pos[2 * i + 1]);
	}
}

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

		//if (!p->type)
		packetHandlers[header->type](msg);
	}
}