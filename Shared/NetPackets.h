#pragma once
#include "NetShared.h"

enum HeaderTypes : i32
{
	HEADER_GENERIC = 1,
	HEADER_CONNECT,
	HEADER_MOVE,
	HEADER_ENTITY,
	headerLastType
};

enum EntityTypes : i32
{
	ENT_PLAYER = 1,
	entityLastType
};

struct StarterData
{
	i32 packetID;
	i32 headerCount;
};

struct HeaderData
{
	HeaderTypes type;
	i32 count;
};

struct EntityData
{
	EntityTypes type;
	i32 count;
};

struct EntityID
{
	i32 id;
};

enum InputBits
{
	IN_FORWARD = (1 << 0),
	IN_LEFT = (1 << 1),
	IN_BACK = (1 << 2),
	IN_RIGHT = (1 << 3),
	IN_MOUSE1 = (1 << 4)
};

struct MoveData
{
	i32 id;
	Vector2 mousePosition;
	bitInt inputs;
};

struct ConnectData
{

};
