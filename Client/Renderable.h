#pragma once
#include "SFML/Graphics.hpp"

class Renderable
{
public:
	virtual void _render(sf::RenderWindow& window) = 0;
};