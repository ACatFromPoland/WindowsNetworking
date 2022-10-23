#pragma once
#include "NetShared.h"

enum HeaderTypes : u8
{
	HEADER_GENERIC = 1,
	HEADER_CONNECT,
	HEADER_MOVE,
	HEADER_ENTITY,
	headerLastType
};

enum EntityTypes : u8
{
	ENT_EMPTY = 1,
	ENT_BOT,
	ENT_PLAYER,
	ENT_ROCKET,
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
	i32 count;
};

struct EntityID
{
	bool remove : 1;
	u32 id : 31;
	EntityTypes type;
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
	u32 id;
	Vector2 mousePosition;
	bitInt inputs;
};

struct ConnectData
{
	u8 classType;
};
