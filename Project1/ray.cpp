#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>


using namespace sf;

ray::ray(float x1, float y1, float x2, float y2) {
	this->line[0] = Vector2f(x1, y1);
	this->line[1] = Vector2f(x2, y2);
}
ray::ray() {}

void ray::setAngle(float angle) {
	this->angle = angle;
}

float ray::getAngle() const {
	return this->angle;
}


void ray::setHead(float x, float y) {
	this->line[0].position = Vector2f(x, y);
}
void ray::setTail(float x, float y) {
	this->line[1].position = Vector2f(x, y);
}

const Vertex* ray::getLine() const {
	return line;
}

float ray::getColumn() const {
	return this->column;
}

void ray::setColumn(const float& column) {
	this->column = column;
}


