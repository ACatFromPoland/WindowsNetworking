#pragma once
#include "SFML/Graphics.hpp"
#include "Shared/Network/NetPlayer.h"
#include "Renderable.h"

class Player : public NetPlayer, public Renderable
{
public:

	virtual void _update();
	virtual void _render(sf::RenderWindow& window);
};