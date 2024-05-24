#include "wall.h"
#include "SFML\Graphics.hpp"
#include <cmath>
#include <iostream>
#include "global.h"



Wall::Wall() {}

sf::RectangleShape Wall::getWall() const {
	return this->shape;
}


float Wall::adjustedDistance(float distance, float column) {
	float adjustedAngle = 90 - (60 + column);
	if (adjustedAngle > 90)
		adjustedAngle = 90 - adjustedAngle;

	float adjustedDistance = distance * cos(adjustedAngle * PI / 180);

	return adjustedDistance;
}


 

void Wall::CC(float distance, float column, float percentOfWall) {

	distance = adjustedDistance(distance, column);
	this->distance = distance;
	this->alpha = 255 - distance;
	this->height = wallHeight / distance * projectionPlaneDistance;
	this->width = WindowTwoWidth / rayCount;
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(column * 20.f, WindowTwoHeight / 2 - (this->height / 2));
	this->shape.setFillColor(sf::Color(223, 0, 132, this->alpha));

	this->columnToTexture = this->customRound(percentOfWall); // because it might become 64 when max index is 63

}


float Wall::getColumnToTexture() {
	return this->columnToTexture;
}



int Wall::customRound(float num) {
	float convert = 63 * num; // this might be correct so that the index doesnt go out of bounds
	return (convert - std::floor(convert) < 0.5) ? std::floor(convert) : std::ceil(convert);
}


float Wall::getDistance() {
	return this->distance;
}

float Wall::getHeight() {
	return this->height;
}