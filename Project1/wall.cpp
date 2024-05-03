#include "wall.h"
#include "SFML\Graphics.hpp"
#define M_PI 3.14159265358979323846 
Wall::Wall(float distance, int column) {

	if (distance > 255)
		return;

	distance = adjustedDistance(distance, column);

	this->alpha = 255 - distance;
	float percentage = (255 - distance) / 255;
	this->height = percentage * 800;
	this->width = 1200 / 60;
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(column * 20.f, 800.f / 2 - (this->height / 2));
	this->shape.setFillColor(sf::Color(255, 255, 255, this->alpha));
}

sf::RectangleShape Wall::getWall() const {
	return this->shape;
}

float Wall::adjustedDistance(float distance, int column) {
	float adjustedAngle = 90 - (60 + column);
	if (adjustedAngle > 90)
		adjustedAngle = 90 - adjustedAngle;

	float adjustedDistance = distance * cos(adjustedAngle * M_PI / 180);

	return adjustedDistance;
}