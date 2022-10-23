#pragma once
#include "Shared/NetPlayer.h"

class Player :  public NetPlayer
{
public:
	Player();
	bool isBot;

	virtual bool isPlayer() { return true; }
	virtual void update();
	void draw();
};