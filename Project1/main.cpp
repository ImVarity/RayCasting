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
#include "global.h"


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

int main()
{
    loadTexturesAndSprites();


    // 2d window
    RenderWindow window(VideoMode(WindowOneWidth, WindowOneHeight), "Window", Style::Default);
    window.setFramerateLimit(144);


    // 3d window
    RenderWindow window2(VideoMode(WindowTwoWidth, WindowTwoHeight), "Window2", Style::Default);
    window2.setFramerateLimit(144);




    // the lines start and end points as walls
    std::vector<std::array<Vertex, 2>> walls;



    // the columns on the projection screen
    Wall columns[120];

    // the rays in window1
    ray rays[720];


    // each next ray increments by .5 degrees
    // might have to use more rays (doom uses 320 rays for 60 FOV)
    float count = 0;
    while (count < 360) {
        rays[int(count * 2)].setAngle(count);
        count += .5;
    }
    



    Sprite emptySPRITE;
    Image emptyIMG;
    Texture emptyTEXTURE;
    emptyIMG.create(1, 64);
    for (int y = 0; y < 64; y++)
        emptyIMG.setPixel(0, y, sf::Color(0, 0, 0));
    emptyTEXTURE.loadFromImage(emptyIMG);
    emptySPRITE.setTexture(emptyTEXTURE);

    Map map;
    map.createMap(walls);


    float rayLength = 1000.f;

    Player player(WindowOneWidth / 2, WindowOneHeight / 2, 1.f);

    CircleShape playerShape = player.getShape();
    float playerOffset = player.getOffset();



    CircleShape hit(15.f, 15.f);

    hit.setPosition(Vector2f(400.f, 400.f));

    Vertex random(Vector2f(1.f, 1.f));
    Vertex random2(Vector2f(1.f, 1.f));


    while (window.isOpen() && window2.isOpen())
    {

        Vector2i mousePositionInt = Mouse::getPosition(window);
        Vector2f mousePosition(static_cast<float>(mousePositionInt.x), static_cast<float>(mousePositionInt.y));


        Event event, event2;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            player.handleKeys(event);

        }

        while (window2.pollEvent(event2)) {
            if (event2.type == Event::Closed)
                window2.close();
        }

        player.handleMovement(map.getMap());
        
        // Clear
        window.clear();
        window2.clear();



        playerShape.setPosition(Vector2f(player.getDX(), player.getDY()));




        
        for (int i = 0; i < FOV * 2; i++) {

            float tempAngle = float(i) / 2 + player.getAngle();
            if (tempAngle >= 360) tempAngle -= 360;
            if (tempAngle < 0) tempAngle += 360;


            Vector2f rayDir(cos(tempAngle * PI / 180), sin(tempAngle * PI / 180));
            Vector2f rayStart(playerShape.getPosition().x + playerOffset, playerShape.getPosition().y + playerOffset);
            Vector2f Step;
            Vector2f Traveled;
            Vector2f rayUnitStepSize = { sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)), sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y)) };
            Vector2i MapLoc(rayStart.x / STEP, rayStart.y / STEP);

            if (rayDir.x < 0) // the ray is pointing left
            {
                Step.x = -1;
                Traveled.x = (rayStart.x - float(MapLoc.x) * STEP) * rayUnitStepSize.x;

            }
            else // the ray is pointing right
            {
                Step.x = 1;
                Traveled.x = (float(MapLoc.x + 1) * STEP - rayStart.x) * rayUnitStepSize.x;
            }
            if (rayDir.y < 0) // the ray is pointing up
            {
                Step.y = -1;
                Traveled.y = (rayStart.y - float(MapLoc.y) * STEP) * rayUnitStepSize.y;
            }
            else // the ray is pointing down
            {
                Step.y = 1;
                Traveled.y = (float(MapLoc.y + 1) * STEP - rayStart.y) * rayUnitStepSize.y;
            }


            bool bTileFound = false;
            float maxDistance = 1200.f;
            float distance = 0.f;
            float sideHit; // 1 = Hit vertical wall, 2 = Hit horizontal wall

            while (!bTileFound && distance < maxDistance)
            {
                if (Traveled.x < Traveled.y)
                {
                    MapLoc.x += Step.x;
                    distance = Traveled.x;
                    Traveled.x += rayUnitStepSize.x * STEP;
                    sideHit = 1;
                }
                else
                {
                    MapLoc.y += Step.y;
                    distance = Traveled.y;
                    Traveled.y += rayUnitStepSize.y * STEP;
                    sideHit = 0;
                }

                if (MapLoc.x >= 0 && MapLoc.x < gridLength && MapLoc.y >= 0 && MapLoc.y < gridLength)
                {
                    if (map.getMap()[MapLoc.y][MapLoc.x] == 1)
                    {
                        bTileFound = true;
                    }
                }
            }

            Vector2f intersection(rayStart + rayDir * distance);

            float T = 1;

            rays[i].setHead(rayStart.x, rayStart.y);
            rays[i].setTail(intersection.x, intersection.y);


            if (sideHit == 1)
                T = std::fmod(intersection.y, 64.0f) / 64.f;
            if (sideHit == 0)
                T = std::fmod(intersection.x, 64.0f) / 64.f;


            columns[i].CC(distance, float(i) / 2, T);


        }



        // Draw
        window.draw(playerShape);
        window.draw(hit);

        // draw the 2d walls
        for (const auto& wall : walls) {
            window.draw(&wall[0], 2, sf::Lines);
        }

        // draw the rays
        for (float i = 0; i < FOV; i += 0.5) {
            window.draw(rays[int(i * 2)].getLine(), 2, Lines);
        }

        // draw the textures on the 3d walls
        for (int i = 0; i < 120; i++) {
            Wall currentWall = columns[i];


            sf::Sprite wallSPRITE = RedBrickSprites[int(currentWall.getColumnToTexture())];

            if (currentWall.getDistance() > 255)
                wallSPRITE = emptySPRITE;

            wallSPRITE.setScale(10.f, currentWall.getHeight() / 64.f);
            wallSPRITE.setPosition(currentWall.getWall().getPosition().x, 800.f / 2 - (currentWall.getHeight() / 2));

            window2.draw(wallSPRITE);
        }


        // drawing a random brick on window1
        for (int i = 0; i < 64; i++) {
            window.draw(RedBrickSprites[i]);
        }



        window.display();
        window2.display();


    }



    return 0;
}

