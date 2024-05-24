#include "global.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

Image loadIMGS(std::string filename);
void loadTexturesAndSprites();

int WindowOneWidth = 1024;
int WindowOneHeight = 1024;

int WindowTwoWidth = 1200;
int WindowTwoHeight = 750;


int FOV = 60;
int rayCount = 320;

double PI = 3.14159265358979323846;

int boxLength = 64;
int gridLength = 16;
int STEP = 64;

int playerHeight = 32;
// A projection plane distance for 320 x 200 game is 277 so scaled by (WindowTwoWidth / rayCount)
float projectionPlaneDistance = 277 * (WindowTwoWidth / rayCount);
int wallHeight = 64;


Image Images[2];
Texture Textures[2][64];
Sprite Sprites[2][64];


Sprite emptySPRITE;

void loadTexturesAndSprites() {
    // red brick
    Images[0].loadFromFile("redbrick.png");
    // doom skull
    Images[1].loadFromFile("doomskull.png");


    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 64; j++) {
            Textures[i][j].loadFromImage(Images[i], sf::IntRect(j, 0, 1, 64));
            Sprites[i][j].setTexture(Textures[i][j]);
            Sprites[i][j].setPosition(i * 64 + 100, 500);
        }
    }



    // Empty sprite
    Image emptyIMG;
    Texture emptyTEXTURE;
    emptyIMG.create(1, 64);
    for (int y = 0; y < 64; y++)
        emptyIMG.setPixel(0, y, sf::Color(0, 0, 0));
    emptyTEXTURE.loadFromImage(emptyIMG);
    emptySPRITE.setTexture(emptyTEXTURE);




}

Image loadIMGS(std::string filename) {
	Image image;
	if (!image.loadFromFile(filename))
		std::cout << "couldnt load img" << std::endl;
	return image;

}