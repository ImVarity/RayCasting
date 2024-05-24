#pragma once
#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>
#include <vector>
#include <array>

class Map {
public:
	Map();

	std::vector<std::vector<int>>& getMap();

	void createMap(std::vector<std::array<Vertex, 2>>& walls);
	void handleMap(Event event, Vector2i mousePosition, std::vector<std::array<Vertex, 2>>& walls);
	void changeBlock(int x, int y);

private:
	void defaultMap();

	float boxlength = 64.f;
	int width;
	int height;
	std::vector<std::vector<int>> map;
};