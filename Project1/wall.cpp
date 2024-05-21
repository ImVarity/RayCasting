#include "wall.h"
#include "SFML\Graphics.hpp"
#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846 



Wall::Wall() {

}

Wall::Wall(float distance, float column, float percentOfWall, const sf::Color textures[][64]) {

	if (distance > 255)
		return;

	distance = adjustedDistance(distance, column);
	this->alpha = 255 - distance;
	float percentage = (255 - distance) / 255;
	this->height = percentage * 800;
	this->width = 1200 / 120;
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(column * 20.f, 800.f / 2 - (this->height / 2));
	this->shape.setFillColor(sf::Color(223, 0, 132, this->alpha));

	int columnToTexture = this->customRound(percentOfWall);


	sf::Image image;
	image.create(1, 64);

	for (int y = 0; y < 64; y++) {
		image.setPixel(0, y, textures[columnToTexture][y]);
	}

	this->img = image;

	//sf::Texture columnTexture;
	//columnTexture.loadFromImage(image);

	//sf::Sprite sprite(columnTexture);
	//sprite.setPosition(column * 20.f, 0);

	//this->sprite = sprite;


}

sf::RectangleShape Wall::getWall() const {
	return this->shape;
}


void Wall::drawWall3D(float distance, float column) {
	if (distance > 255)
		return;

	distance = adjustedDistance(distance, column);

	this->alpha = 255 - distance;
	float percentage = (255 - distance) / 255;
	this->height = percentage * 800;
	this->width = 1200 / 120;
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(column * 20.f, 800.f / 2 - (this->height / 2));
	this->shape.setFillColor(sf::Color(255, 255, 255, this->alpha));



}

float Wall::adjustedDistance(float distance, float column) {
	float adjustedAngle = 90 - (60 + column);
	if (adjustedAngle > 90)
		adjustedAngle = 90 - adjustedAngle;

	float adjustedDistance = distance * cos(adjustedAngle * M_PI / 180);

	return adjustedDistance;
}


sf::Sprite Wall::wallTexture(float percentOfWall, float column3D, const sf::Color texture[][64]) {


	int columnToTexture = this->customRound(percentOfWall);



	sf::Image image;
	image.create(1, 64);

	for (int y = 0; y < 64; y++) {
		image.setPixel(0, y, texture[columnToTexture][y]);
	}

	sf::Texture columnTexture;
	columnTexture.loadFromImage(image);

	sf::Sprite sprite(columnTexture);
	sprite.setPosition(column3D * this->width, 0);


	return sprite;
}




void Wall::CC(float distance, float column, float percentOfWall, const sf::Color textures[][64]) {


	this->distance = distance;

	if (distance > 255) {
		this->shape.setFillColor(sf::Color(255, 255, 255));
		//this->img = emptyWall();
	}


	distance = adjustedDistance(distance, column);

	this->alpha = 255 - distance;
	float percentage = (255 - distance) / 255;
	this->percentage = (255 - distance) / 255;
	this->height = percentage * 800;
	this->width = 1200 / 120;
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(column * 20.f, 800.f / 2 - (this->height / 2));
	this->shape.setFillColor(sf::Color(223, 0, 132, this->alpha));

	int columnToTexture = this->customRound(percentOfWall);
	this->columnToTexture = columnToTexture; // because it might become 64 when max index is 63

	//sf::Image image;
	//image.create(1, 64);

	//for (int y = 0; y < 64; y++) {
	//	image.setPixel(0, y, textures[columnToTexture][y]);
	//}

	//this->img = image;
}


float Wall::getColumnToTexture() {
	return this->columnToTexture;
}

float Wall::getPercentage() {
	return this->percentage;
}

float Wall::getHeight() {
	return this->height;
}

int Wall::customRound(float num) {
	float convert = 63 * num; // this might be correct so that the index doesnt go out of bounds
	return (convert - std::floor(convert) < 0.5) ? std::floor(convert) : std::ceil(convert);
}


sf::Sprite Wall::getTexture() {
	return this->sprite;
}

sf::Image Wall::getImage() {
	return this->img;
}

sf::Image Wall::emptyWall() {
	sf::Image empty;
	empty.create(1, 64);

	for (int y = 0; y < 64; y++) {
		empty.setPixel(0, y, sf::Color(0, 0, 0));
	}

	return empty;
}

float Wall::getDistance() {
	return this->distance;
}