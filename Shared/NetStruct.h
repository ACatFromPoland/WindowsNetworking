#pragma once
#include "Utils.h"

enum PacketType : byte {
	CLIENT_CONNECT = 0,
	SERVER_DATA,
	CLIENT_INPUT,
	SERVER_WORLD,
	ECHO_RESPOND,
	lastType
};

struct StarterPacket
{
	int ID;
	// TODO: use checksum
	int checkSum;
};

struct HeaderPacket
{
	PacketType type;
};

struct ResponsePacket
{
	int bytes;
};

struct ConnectPacket
{
	char name[64];
};

struct InitPacket
{
	int ID;
	int playerCount;
};

struct InputPacket 
{
	int ID;
	bool input[4];
};

// TODO: Make World Packet Dynamic size
struct WorldPacket
{
	bool active[8];
	float pos[8 * 2];
};