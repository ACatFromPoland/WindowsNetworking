#include "c_player.h"

void Player::_update()
{
	return;
}

void Player::_render(sf::RenderWindow& window)
{
	sf::CircleShape circle;
	sf::Color color;
	float radius = 0.0f;
	switch (classId)
	{
		case 0:
		{
			color = sf::Color(74, 72, 68);
			radius = 5.0f;
			break;
		}
		case 1:
		{
			color = sf::Color(113, 113, 115);
			radius = 6.0f;
			break;
		}
		
		case 2:
		{
			color = sf::Color(91, 47, 50);
			radius = 6.0f;
			break;
		}
		case 3:
		{
			color = sf::Color(185, 122, 87);
			radius = 6.5f;
			break;
		}
		case 4:
		{
			color = sf::Color(118, 99, 76);
			radius = 8.0f;
			break;
		}
		case 5:
		{
			color = sf::Color(255, 127, 39);
			radius = 6.5f;
			break;
		}
		case 6:
		{
			color = sf::Color(203, 180, 165);
			radius = 5.0f;
			break;
		}
		case 7:
		{
			color = sf::Color(44, 33, 30);
			radius = 4.8f;
			break;
		}
		case 8:
		{
			color = sf::Color(118, 82, 76);
			radius = 4.5f;
			break;
		}
	}

	circle.setFillColor(color);
	circle.setRadius(radius);
	circle.setPosition(position.value.x, position.value.y);
	window.draw(circle);
}