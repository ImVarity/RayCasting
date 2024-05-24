#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <SFML/Graphics.hpp>
#include <array>


using namespace sf;

extern int WindowOneWidth;
extern int WindowOneHeight;

extern int WindowTwoWidth;
extern int WindowTwoHeight;

extern int FOV;
extern int rayCount;
extern int wallCount;

extern double PI;

extern int boxLength;
extern int gridLength;

extern int STEP;

extern int playerHeight;
extern float projectionPlaneDistance;

extern int wallHeight;



extern Image Images[2];
extern Texture Textures[2][64];
extern Sprite Sprites[2][64];





extern Sprite emptySPRITE;



void loadTexturesAndSprites();
Image loadIMGS(std::string filename);


#endif




