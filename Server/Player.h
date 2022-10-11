#pragma once
#include "Entity.h"
#include "Shared/Vector2.h"

/*
W
A
S
D
SPACE
1
2
3
4
M1
M2
*/

class Player : public Entity
{
public:
	const static int playerInputsSize = 4;
	static int ID;

	sockaddr_in client;
	
	NetByte(classId);
	NetByte(weaponId);
	NetByte(health);

	Player(int id, Vector2 pos, Vector2 vel);

	virtual void _update();
};
