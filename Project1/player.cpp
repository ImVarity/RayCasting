#include <iostream>
#include "player.h"
#define M_PI 3.14159265358979323846 
#define windowTWO {1200, 800}
#define windowONE { 16 * 64, 16 * 64 }

Player::Player(float x, float y) : shape(this->radius) {
	this->location = Vector2f(x, y);
}

void Player::handleKeys(Event& event) {
    if (Keyboard::isKeyPressed(Keyboard::W))
        this->moveUp = true;
    else
        this->moveUp = false;

    if (Keyboard::isKeyPressed(Keyboard::A))
        this->moveLeft = true;
    else
        this->moveLeft = false;

    if (Keyboard::isKeyPressed(Keyboard::S))
        this->moveDown = true;
    else
        this->moveDown = false;

    if (Keyboard::isKeyPressed(Keyboard::D))
        this->moveRight = true;
    else
        this->moveRight = false;

    if (Keyboard::isKeyPressed(Keyboard::E))
        this->rotateClockwise = true;
    else
        this->rotateClockwise = false;

    if (Keyboard::isKeyPressed(Keyboard::Q))
        this->rotateCounterClockwise = true;
    else
        this->rotateCounterClockwise = false;
}


void Player::handleMovement() {

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


float Player::getDX() const {
    return this->dx;
}

float Player::getDY() const {
    return this->dy;
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

void Player::setCurrGridLoc(Vector2f loc) {
    this->currGridLoc = loc;
}

void Player::setPrevGridLoc(Vector2f loc) {
    this->prevGridLoc = loc;
}

void Player::handleLocs(Vector2f currentLoc) {
    
    if (this->queueLoc.size() < 2)
        this->queueLoc.push(currentLoc);
    else {
        if (this->queueLoc.back() == currentLoc)
            return;
        else
        {
            this->queueLoc.push(currentLoc);
            this->queueLoc.pop();
        }
    }


}

Vector2f Player::getLocation() const {
    return this->location;
}


bool Player::collide(std::vector<std::vector<int>>& map) {

    Vector2f prevLoc = this->queueLoc.front();
    Vector2f currLoc = this->queueLoc.back();

    float differenceX = currLoc.x - prevLoc.x;
    float differenceY = currLoc.y - prevLoc.y;


    if (map[currLoc.y][currLoc.x] == 1) {


        if (differenceX == 1 && differenceY == 0) {
            std::cout << "colliding right wall" << std::endl;
            this->moveRight = false;
        }
        if (differenceX == -1 && differenceY == 0) {
            std::cout << "colliding left wall" << std::endl;
            this->moveLeft = false;
        }
        if (differenceX == 0 && differenceY == -1) {
            std::cout << "colliding top wall" << std::endl;
            this->moveUp = false;
        }
        if (differenceX == 0 && differenceY == 1) {
            std::cout << "colliding bottom wall" << std::endl;
            this->moveDown = false;
        }
    }


    
     


    return true;






}