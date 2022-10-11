//TODO: Store array size in header \/HERE
enum HeaderType : unsigned char
{
	SERVER_INFO = 0,
	CLIENT_INFO,
	CLIENT_INPUT,
	ENTITY,
	LAST_HEADERTYPE
};

enum EntityTypes : int
{
	ENTITY = 0,
	PLAYER,
	PROJECTILE,
	LAST_ENTITYTYPE
};