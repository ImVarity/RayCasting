#pragma once
#include "SFML\Graphics.hpp"
#include <unordered_map>


class Wall {
public:
	Wall();
	Wall(float distance, float column, float percentOfWall, const sf::Color textures[][64]);
	
	void drawWall3D(float distance, float column);


	sf::RectangleShape getWall() const;
	sf::Sprite wallTexture(float percentOfWall, float column3D, const sf::Color texture[][64]);



	sf::Sprite getTexture();
	sf::Image getImage();

	void CC(float distance, float column, float percentOfWall, const sf::Color textures[][64]);


	float getPercentage();
	float getHeight();
	float getDistance();
	float getColumnToTexture();


	sf::Image emptyWall();

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

	sf::Sprite sprite;

	sf::Image img;

	sf::RectangleShape shape;
};