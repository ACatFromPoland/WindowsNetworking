#pragma once
#include "ANet/Server.h"
#include "Shared/NetClock.h"
#include "sEntityHandler.h"

#include "sPlayer.h"
#include "sRocket.h"

class ColliderBase
{
public:

};

class BoxCollider : public ColliderBase
{
public:

};

class Ray : public ColliderBase
{
public:

};

class World
{
public:
	NetServer* server;
	NetClock clock;

	EntityHandler entityHandler;

	World(NetServer* network);

	Player* getPlayer(u32 id);
	Player* createPlayer(sockaddr_in address, u8 classType);
	void checkConnections();
	void update();
	void updateClients();
	
	bool collides(Vector2 point);
	bool collides(BoxCollider box);
	bool collides(Ray ray);
};