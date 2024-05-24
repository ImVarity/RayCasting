#include <iostream>
#include <stdlib.h>
#include "player.h"

#define M_PI 3.14159265358979323846 
#define windowTWO {1200, 800}
#define windowONE { 16 * 64, 16 * 64 }

Player::Player(float x, float y, float speed) : shape(10.f) {
	this->location = Vector2f(x, y);
    this->speed = speed;
}

void Player::handleKeys(Event& event) {

    this->moveUp = Keyboard::isKeyPressed(Keyboard::W) ? true : false;
    this->moveDown = Keyboard::isKeyPressed(Keyboard::S) ? true : false;
    this->moveLeft = Keyboard::isKeyPressed(Keyboard::A) ? true : false;
    this->moveRight = Keyboard::isKeyPressed(Keyboard::D) ? true : false;


    if (Keyboard::isKeyPressed(Keyboard::E))
        this->rotateClockwise = true;
    else
        this->rotateClockwise = false;

    if (Keyboard::isKeyPressed(Keyboard::Q))
        this->rotateCounterClockwise = true;
    else
        this->rotateCounterClockwise = false;
}


void Player::handleMovement(std::vector<std::vector<int>>& map) {

    (checkCollision(this->moveUp, this->moveDown, this->moveLeft, this->moveRight, map));

    if (this->moveUp && this->moveRight) // Diagonally up right
        this->location += Vector2f(cos(45 * M_PI / 180), -sin(45 * M_PI / 180)) * this->speed;
    else if (this->moveUp && this->moveLeft) // Diagonally up left
        this->location += Vector2f(-cos(45 * M_PI / 180), -sin(45 * M_PI / 180)) * this->speed;
    else if (this->moveDown && this->moveRight) // Diagonally down right
        this->location += Vector2f(cos(45 * M_PI / 180), sin(45 * M_PI / 180)) * this->speed;
    else if (this->moveDown && this->moveLeft) // Diagonally down left
        this->location += Vector2f(-cos(45 * M_PI / 180), sin(45 * M_PI / 180)) * this->speed;

    else if (this->moveUp)
        this->location -= Vector2f(0.f, this->speed);
    else if (this->moveDown)
        this->location += Vector2f(0.f, this->speed);
    else if (this->moveLeft)
        this->location -= Vector2f(this->speed, 0.f);
    else if (this->moveRight)
        this->location += Vector2f(this->speed, 0.f);


    if (this->rotateCounterClockwise)
        this->angle -= 1;
    if (this->rotateClockwise)
        this->angle += 1;

    if (this->angle >= 360)
        this->angle -= 360;
    if (this->angle < 0)
        this->angle += 360;




    this->dx = cos(this->angle * M_PI / 180);
    this->dy = sin(this->angle * M_PI / 180);


    this->dx += this->location.x;
    this->dy += this->location.y;

    

}

void Player::setSpeed(float speed) {
    this->speed = speed;
}

void Player::setLocation(const Vector2f& loc) {
    this->location = loc;
}


float Player::getDX() const {
    return this->dx;
}

float Player::getDY() const {
    return this->dy;
}

float Player::getRadius() const {
    return this->radius;
}

float Player::getAngle() const {
    return this->angle;
}

CircleShape Player::getShape() const {
    return this->shape;
}

float Player::getOffset() const {
    return this->radius;
}


Vector2f Player::getLocation() const {
    return this->location;
}


void Player::checkCollision(bool up, bool down, bool left, bool right, std::vector<std::vector<int>>& map) {

    float directionX = (left ? -this->speed : 0) + (right ? this->speed : 0);
    float directionY = (up ? -this->speed : 0) + (down ? this->speed : 0);
    
    float X = this->location.x;
    float Y = this->location.y;

    for (int i = 0; i < 4; i++)
    {
        if (i == 0) {
            X = this->location.x;
            Y = this->location.y - 10.f;
        }
        else if (i == 1) {
            X = this->location.x + 10.f;
            Y = this->location.y;
        }
        else if (i == 2) {
            X = this->location.x;
            Y = this->location.y + 10.f;
        }
        else if (i == 3) {
            X = this->location.x - 10.f;
            Y = this->location.y;
        }

        X += this->radius;
        Y += this->radius;
        float nextHorizontal = X + directionX;
        float nextVertical = Y + directionY;

        int nextGridHorizontal = nextHorizontal / 64;
        int nextGridVertical = nextVertical / 64;


        int currGridHorizontal = X / 64;
        int currGridVertical = Y / 64;



        if (map[nextGridVertical][nextGridHorizontal] == 1) // about to enter a wall
        {
            bool collidingDown = nextGridVertical - 1 == currGridVertical;
            bool collidingUp = nextGridVertical + 1 == currGridVertical;
            bool collidingRight = nextGridHorizontal - 1 == currGridHorizontal;
            bool collidingLeft = nextGridHorizontal + 1 == currGridHorizontal;


            if (std::abs(currGridHorizontal - nextGridHorizontal) == 1 && std::abs(currGridVertical - nextGridVertical) == 1) { // about to enter a wall diagonally
                if (map[nextGridVertical][currGridHorizontal] == 0) { // allowed to move vertically
                    if (collidingRight)
                        this->moveRight = false;
                    else if (collidingLeft)
                        this->moveLeft = false;

                }

                else if (map[currGridVertical][nextGridHorizontal] == 0) { // allowed to move horizontally
                    if (collidingUp)
                        this->moveUp = false;
                    else if (collidingDown)
                        this->moveDown = false;
                }
                else {
                    if (collidingUp)
                        this->moveUp = false;
                    else if (collidingDown)
                        this->moveDown = false;
                    if (collidingRight)
                        this->moveRight = false;
                    else if (collidingLeft)
                        this->moveLeft = false;
                }

            }
            else if (std::abs(currGridVertical - nextGridVertical) == 1) { // about to hit something vertically
                if (collidingUp)
                    this->moveUp = false;
                else if (collidingDown)
                    this->moveDown = false;
            }

            else { // about to hit something horizontall
                if (collidingLeft)
                    this->moveLeft = false;
                else if (collidingRight)
                    this->moveRight = false;
            }

        }


    }
   

}