#pragma once
#include "SFML\Graphics.hpp"

class Wall {
public:
	Wall(float distance, int column);
	sf::RectangleShape getWall() const;

private:
	float adjustedDistance(float distance, int column); // Fixes the fisheye


	float width = 0;
	float height = 0;
	float alpha = 255;


	sf::RectangleShape shape;
};