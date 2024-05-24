#pragma once
#include "SFML\Graphics.hpp"
#include <unordered_map>


class Wall {
public:
	Wall();
	

	sf::RectangleShape getWall() const;

	void CC(float distance, float column, float percentOfWall);


	float getHeight();
	float getDistance();
	float getColumnToTexture();


private:
	float adjustedDistance(float distance, float column); // Fixes the fisheye
	int customRound(float num);


	float closestDistance;
	float width = 0;
	float height = 0;
	float alpha = 255;
	float columnPosition;
	float percentage;
	float distance;
	float columnToTexture;

	sf::RectangleShape shape;
};