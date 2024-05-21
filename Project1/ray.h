#pragma once
#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>

using namespace sf;

class ray {
public:
	ray();
	ray(float x1, float y1, float x2, float y2);

	void setHead(float x1, float y1);
	void setTail(float x2, float y2);
	void setAngle(float angle);
	float getAngle() const;

	float getColumn() const;
	void setColumn(const float& column);


	const Vertex* getLine() const;


private:
	Vertex line[2];
	float x1;
	float y1;
	float x2;
	float y2;
	float angle = 0;

	float column;

};