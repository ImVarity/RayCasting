#pragma once
#include "SFML\Graphics.hpp"


class Wall {
public:
	Wall();
	

	sf::RectangleShape getWall() const;

	void CC(float distance, float column, float percentOfWall);


	float getHeight();
	float getDistance();
	float getColumnToTexture();

	void setTextureNumber(int number);
	int getTextureNumber() const;

private:
	float adjustedDistance(float distance, float column); // Fixes the fisheye
	int customRound(float num);


	float closestDistance;
	float width = 0;
	float height = 0;
	float alpha = 255;
	float distance;
	float columnToTexture;

	// This is the wall texture going to be used
	int textureNumber;

	sf::RectangleShape shape;
};