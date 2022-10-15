#include "s_Player.h"

int Player::ID = 0;

Player::Player(int id, Vector2 pos, Vector2 vel)
{
	classId = 0;
	weaponId = 0;
	health = 0;
}

void Player::_update()
{
	position.value.x += 1.0f;
}