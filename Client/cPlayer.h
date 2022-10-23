#pragma once
#include "Shared/NetPlayer.h"

class Player :  public NetPlayer
{
public:
	Player();

	virtual bool isPlayer() { return true; }
	virtual void update();
	void draw();
};