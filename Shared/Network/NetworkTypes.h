//TODO: Store array size in header \/HERE
#include "ANet/NetBuffer.h"

enum HeaderType : unsigned char
{
	GENERIC = 0,
	CONNECT,
	ENTITY,
	MOVE,
	LAST_HEADERTYPE
};

enum EntityTypes : unsigned char
{
	HUSK = 0,
	PLAYER,
	PROJECTILE,
	LAST_ENTITYTYPE
};