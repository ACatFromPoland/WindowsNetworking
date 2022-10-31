#pragma once
#include "NetShared.h"

#define ENTITY_ID u16

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
	u32 packetID;
	u32 headerCount;
};

struct HeaderData
{
	HeaderTypes type;
	u32 count;
};

struct EntityData
{
	u32 count;
};

struct EntityID
{
	bool remove : 1;
	ENTITY_ID id : 15;
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
	ENTITY_ID id;
	Vector2 mousePosition;
	bitInt inputs;
};

struct ConnectData
{
	u8 classType;
};

struct InitData
{
	ENTITY_ID id;
};

struct ActivityData
{
	ENTITY_ID id;
};