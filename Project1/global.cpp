#include "global.h"
#include <iostream>
#include <string>

Image loadIMGS(std::string filename);
void loadTexturesAndSprites();

int WindowOneWidth = 1024;
int WindowOneHeight = 1024;

int WindowTwoWidth = 1200;
int WindowTwoHeight = 800;


int FOV = 60;
//constexpr int rayCount = 720; NNED TO FIX
int columnCount = 120;

double PI = 3.14159265358979323846;

int boxLength = 64;
int gridLength = 16;

int STEP = 64;

int playerHeight = 32;


Image RedBrickImg;
Texture RedBrickTextures[64];
Sprite RedBrickSprites[64];

Sprite emptySPRITE;

void loadTexturesAndSprites() {
    RedBrickImg.loadFromFile("redbrick.png");
    for (int i = 0; i < 64; i++) {
        RedBrickTextures[i].loadFromImage(RedBrickImg, sf::IntRect(i, 0, 1, 64));
        RedBrickSprites[i].setTexture(RedBrickTextures[i]);
        RedBrickSprites[i].setPosition(i + 500, 500);
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