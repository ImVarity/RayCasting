#pragma once
#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>
#include <vector>
#include <array>
#include "SFML\Graphics.hpp"
#include "queue"


class Player {
public:
	Player(float x, float y, float speed);
	
	void handleKeys(Event& event);
	void handleMovement(std::vector<std::vector<int>>& map);

	CircleShape getShape() const;
	float getDX() const;
	float getDY() const;
	float getOffset() const;
	float getAngle() const;
	float getRadius() const;

	void setSpeed(float speed);
	void setLocation(const Vector2f& loc);

	Vector2f getLocation() const;



private:

	void checkCollision(bool up, bool down, bool left, bool right, std::vector<std::vector<int>>& map);

	Vector2f location;
	CircleShape shape;

	float radius = 10.f;

	float dx = 0;
	float dy = 0;

	float angle = 0;

	float speed;

	bool moveUp = false;
	bool moveDown = false;
	bool moveRight = false;
	bool moveLeft = false;

	bool rotateClockwise = false;
	bool rotateCounterClockwise = false;

};