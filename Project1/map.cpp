#include "map.h"
#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>
#include <vector>
#include <array>

Map::Map() : width(16), height(16) {
	defaultMap();
}


void Map::defaultMap() {
    this->map = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
}


std::vector<std::vector<int>>& Map::getMap() {
    return this->map;
}


void Map::createMap(std::vector<std::array<Vertex, 2>>& walls) {
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++) {

            if (this->map[i][j] == 1)
            {

                float topleftx = j * this->boxlength;
                float toplefty = i * this->boxlength;

                float toprightx = (j + 1) * this->boxlength;
                float toprighty = i * this->boxlength;

                float bottomrightx = toprightx;
                float bottomrighty = toprighty + this->boxlength;

                float bottomleftx = topleftx;
                float bottomlefty = toplefty + this->boxlength;

                std::array<Vertex, 2> topWall = { Vertex(Vector2f(topleftx, toplefty)), Vertex(Vector2f(toprightx, toprighty)) };
                std::array<Vertex, 2> rightWall = { Vertex(Vector2f(toprightx, toprighty)), Vertex(Vector2f(bottomrightx, bottomrighty)) };
                std::array<Vertex, 2> bottomWall = { Vertex(Vector2f(bottomrightx, bottomrighty)), Vertex(Vector2f(bottomleftx, bottomrighty)) };
                std::array<Vertex, 2> leftWall = { Vertex(Vector2f(bottomleftx, bottomlefty)), Vertex(Vector2f(topleftx, toplefty)) };



                if (i - 1 >= 0) // Something on top
                {
                    if (this->map[i - 1][j] == 0) // There is a 0 above the 1
                        walls.push_back(topWall);
                }

                if (j - 1 >= 0) // Something on the left
                {
                    if (this->map[i][j - 1] == 0) // There is a 0 on the left of the 1
                        walls.push_back(leftWall);
                }

                if (i + 1 < this->height) // Something below
                {
                    if (this->map[i + 1][j] == 0) // There is a 0-on the bottom of the 1
                        walls.push_back(bottomWall);
                }

                if (j + 1 < this->width) // Something on the right
                {
                    if (this->map[i][j + 1] == 0) // There is a 0 on the right of the 1
                        walls.push_back(rightWall);
                }

            }


        }
    }

}

void Map::handleMap(Event event, Vector2i mousePosition, std::vector<std::array<Vertex, 2>>& walls) {
    if (event.type == sf::Event::MouseButtonReleased)
    {
        int mouseLocX = mousePosition.x / this->boxlength;
        int mouseLocY = mousePosition.y / this->boxlength;

        this->map[mouseLocY][mouseLocX] = this->map[mouseLocX][mouseLocY] == 0 ? 1 : 0;

    }
}

void Map::changeBlock(int x, int y) {
    this->map[x][y] = this->map[x][y] == 0 ? 1 : 0;
}