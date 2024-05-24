#pragma once
#include "SFML\Graphics.hpp"
#include <unordered_map>


class Wall {
public:
	Wall();
	
	void drawWall3D(float distance, float column);


	sf::RectangleShape getWall() const;
	sf::Sprite wallTexture(float percentOfWall, float column3D, const sf::Color texture[][64]);



	sf::Sprite getTexture();
	sf::Image getImage();

	void CC(float distance, float column, float percentOfWall);


	float getPercentage();
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