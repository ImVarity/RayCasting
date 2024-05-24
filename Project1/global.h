#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <SFML/Graphics.hpp>


using namespace sf;

extern int WindowOneWidth;
extern int WindowOneHeight;

extern int WindowTwoWidth;
extern int WindowTwoHeight;

extern int FOV;
//extern constexpr int rayCount; NEED TO FIX
extern int wallCount;

extern double PI;


extern int boxLength;
extern int gridLength;

extern int STEP;

extern int playerHeight;


extern Image RedBrickImg;
extern Texture RedBrickTextures[64];
extern Sprite RedBrickSprites[64];

extern Sprite emptySPRITE;



void loadTexturesAndSprites();
Image loadIMGS(std::string filename);


#endif




