#include "Entity.h"

Entity::Entity()
{
	id = 0;
	position = { 0.0f, 0.0f };
	velocity = { 0.0f, 0.0f };
}

Entity::Entity(int _id, Vector2 _pos, Vector2 _vel)
{
	id = _id;
	position = _pos;
	velocity = _vel;
}