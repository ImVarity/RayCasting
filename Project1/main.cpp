#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>
#include <vector>
#include <array>
#include <unordered_map>
#include <map>
#include "player.h"
#include "map.h"
#include "wall.h"

#define windowTWO {1200, 800}
#define windowONE { 16 * 64, 16 * 64 }

#define M_PI 3.14159265358979323846 
#define FOV 60

using namespace sf;


void draw3D();

float* lookAtWall(std::array<Vertex, 2> wall, Vector2f head, Vector2f tail) {

    float tu[2];

    float x1 = wall[0].position.x;
    float y1 = wall[0].position.y;
    float x2 = wall[1].position.x;
    float y2 = wall[1].position.y;

    float x3 = head.x;
    float y3 = head.y;
    float x4 = tail.x;
    float y4 = tail.y;


    float num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    tu[0] = num / den;
    tu[1] = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;


    return tu;

}

// so dumb i hate this
struct Vector2uHash {
    std::size_t operator()(const sf::Vector2u& v) const {
        return std::hash<unsigned int>()(v.x) ^ (std::hash<unsigned int>()(v.y) << 1);
    }
};

int main()
{
    

    int gridLength = 16;
    float boxlength = 64.f;

    // 2d window
    int windowSize[] = windowONE;
    RenderWindow window(VideoMode(windowSize[0], windowSize[1]), "Window", Style::Default);
    window.setFramerateLimit(144);


    int wallHeight = 300;
    float wallWidth = 100;

    // 3d window
    int window2Size[] = windowTWO;
    RenderWindow window2(VideoMode(window2Size[0], window2Size[1]), "Window2", Style::Default);
    window2.setFramerateLimit(144);




    // the lines start and end points as walls
    std::vector<std::array<Vertex, 2>> walls;



    Wall walls3D[120];
    Sprite wallTextures[120];


    ray rays[720];

    float count = 0;

    while (count < 360) {
        rays[int(count * 2)].setAngle(count);
        count += .5;
    }



    Map map;
    map.createMap(walls);



    float rayLength = 1000.f;



    Player player(windowSize[0] / 2, windowSize[1] / 2);
    player.setSpeed(1.f);

    CircleShape playerShape = player.getShape();
    float playerOffset = player.getOffset();



    Image image;
    if (!image.loadFromFile("redbrick.png"))
        std::cout << "couldnt load brick" << std::endl;

    Texture texture;
    if (!texture.loadFromImage(image))
        std::cout << "czouldn't load brick" << std::endl;

    Sprite sprite(texture);
    sprite.scale(2.f, 2.f);
    sprite.setPosition(Vector2f(512, 512));


    Vector2u textureSize = texture.getSize();

    Color REDBRICK[64][64];

    // Iterate through each pixel and store its location and color
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            sf::Vector2u pixelLocation(x, y);
            sf::Color pixelColor = image.getPixel(x, y);
            REDBRICK[x][y] = pixelColor;
            //std::cout << static_cast<int>(REDBRICK[x][y].r) << ", "
            //          << static_cast<int>(REDBRICK[x][y].g) << ", "
            //          << static_cast<int>(REDBRICK[x][y].b) << ", "
            //          << static_cast<int>(REDBRICK[x][y].a) << std::endl;
        }
    }


    Texture RedBrickTextures[64];
    Sprite RedBrickSprites[64];
    
    Image emptyIMG;
    Texture emptyTEXTURE;
    Sprite emptySPRITE;
    emptyIMG.create(1, 64);

    for (int y = 0; y < 64; y++) {
        emptyIMG.setPixel(0, y, sf::Color(0, 0, 0));
    }
    emptyTEXTURE.loadFromImage(emptyIMG);
    emptySPRITE.setTexture(emptyTEXTURE);



    for (int i = 0; i < 64; i++) {

        RedBrickTextures[i].loadFromImage(image, sf::IntRect(i, 0, 1, 64));
        RedBrickSprites[i].setTexture(RedBrickTextures[i]);
        RedBrickSprites[i].setPosition(i + 500, 500);


    }


   
    while (window.isOpen() && window2.isOpen())
    {

        Vector2i mousePosition = Mouse::getPosition(window);

        Event event, event2;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            player.handleKeys(event);


        }
        
        player.handleMovement(map.getMap());


        


        while (window2.pollEvent(event2))
        {
            if (event2.type == Event::Closed)
                window2.close();
        }

        // Clear
        window.clear();
        window2.clear();



        playerShape.setPosition(Vector2f(player.getDX(), player.getDY()));


        for (float i = 0; i < FOV * 2; i += 0.5f) {
            float tempAngle = i + player.getAngle();
            if (tempAngle >= 360) tempAngle -= 360;
            if (tempAngle < 0) tempAngle += 360;

            rays[int(i * 2)].setHead(playerShape.getPosition().x + playerOffset, playerShape.getPosition().y + playerOffset);
            rays[int(i * 2)].setTail(playerShape.getPosition().x + cos(tempAngle * M_PI / 180) * rayLength,
            playerShape.getPosition().y + sin(tempAngle * M_PI / 180) * rayLength);

            float closestDistance = std::numeric_limits<float>::max();
            float closestx = 0.0f;
            float closesty = 0.0f;
            float closestt = 0.0f;

            for (int j = 0; j < walls.size(); j++) {
                float* tu = lookAtWall(walls[j], rays[int(i * 2)].getLine()[0].position, rays[int(i * 2)].getLine()[1].position);
                float t = tu[0];
                float u = tu[1];


                if (t > 0 && t < 1 && u > 0) {

                   /* if (i == 0)
                        std::cout << "t: " << t << " u: " << u << std::endl;*/

                    float x1 = walls[j][0].position.x;
                    float y1 = walls[j][0].position.y;
                    float x2 = walls[j][1].position.x;
                    float y2 = walls[j][1].position.y;


                    float hitpointx = x1 + t * (x2 - x1);
                    float hitpointy = y1 + t * (y2 - y1);

                    float distance = std::sqrt((hitpointx - rays[int(i * 2)].getLine()[0].position.x) * (hitpointx - rays[int(i * 2)].getLine()[0].position.x) +
                        (hitpointy - rays[int(i * 2)].getLine()[0].position.y) * (hitpointy - rays[int(i * 2)].getLine()[0].position.y));


                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestx = hitpointx;
                        closesty = hitpointy;
                        closestt = t;
                    }

                }

                
            }

            if (closestDistance != std::numeric_limits<float>::max()) {
                rays[int(i * 2)].setTail(closestx, closesty);
            }


            //std::cout << int(i * 2) << std::endl;
            if (int(i * 2) < 120)
                walls3D[int(i * 2)].CC(closestDistance, i, closestt, REDBRICK);


            //Wall wall(closestDistance, i, closestt, REDBRICK);
            //window2.draw(wall.getWall());



            //walls3D[int(i * 2)].drawWall3D(closestDistance, i);
            //window2.draw(walls3D[int(i * 2)].getWall());


            //window2.draw(wall.wallTexture(closestt, i, REDBRICK));
        }



        // Draw
        window.draw(playerShape);
        //window.draw(sprite);

        // draw the 2d walls
        for (const auto& wall : walls) {
            window.draw(&wall[0], 2, sf::Lines);
        }

        // draw the rays
        for (float i = 0; i < FOV; i += 0.5) {
            window.draw(rays[int(i * 2)].getLine(), 2, Lines);
        }

        for (int i = 0; i < 120; i++) {
            Wall currentWall = walls3D[i];


            sf::Sprite wallSPRITE = RedBrickSprites[int(currentWall.getColumnToTexture())];

            if (currentWall.getDistance() > 255)
                wallSPRITE = emptySPRITE;

            wallSPRITE.setScale(10.f, currentWall.getHeight() / 64.f);
            wallSPRITE.setPosition(currentWall.getWall().getPosition().x, 800.f / 2 - (currentWall.getHeight() / 2));

            //window2.draw(currentWall.getWall());
            window2.draw(wallSPRITE);






        }



        for (int i = 0; i < 64; i++) {
            window.draw(RedBrickSprites[i]);
        }



        window.display();
        window2.display();


    }



    return 0;
}

