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
	int checkSum;
};

struct HeaderPacket
{
	PacketType type;
};

struct ResponsePacket
{
	// TODO use checksum
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

struct WorldPacket
{
	int ids[8];
	float pos[16];
};