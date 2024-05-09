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
	Player(float x, float y);
	
	void handleKeys(Event& event);
	void handleMovement(std::vector<std::vector<int>>& map);

	void setSpeed(float speed);

	float getDX() const;
	float getDY() const;

	float getAngle() const;

	CircleShape getShape() const;

	float getOffset() const;


	bool collide(std::vector<std::vector<int>>& map) ;

	void handleLocs(Vector2f currentLoc);


	void setCurrGridLoc(Vector2f loc);
	void setPrevGridLoc(Vector2f loc);

	Vector2f getLocation() const;



private:

	void checkCollision(bool up, bool down, bool left, bool right, std::vector<std::vector<int>>& map);

	Vector2f location;
	CircleShape shape;

	std::queue<sf::Vector2f> queueLoc;

	Vector2f currGridLoc;
	Vector2f prevGridLoc;

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