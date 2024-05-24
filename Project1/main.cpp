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

#define rayCount 320

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
    Wall columns[320];

    // the rays in window1
    ray rays[320];


    // each next ray increments by .5 degrees
    // might have to use more rays (doom uses 320 rays for 60 FOV)

    float increment = float(FOV) / float(rayCount);
    float count = 0;
    int sampleI = 0;
    while (sampleI < 320) 
    {
        rays[sampleI].setAngle(count);
        count += increment;
        sampleI++;
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


    Player player(WindowOneWidth / 2, WindowOneHeight / 2, 1.f);

    CircleShape playerShape = player.getShape();
    float playerOffset = player.getOffset();



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


        float rayAngle = 0;
        int rayI = 0;
        while (rayI < rayCount) {
            float tempAngle = rayAngle + player.getAngle();
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
                    if (map.getMap()[MapLoc.y][MapLoc.x] > 0)
                    {
                        bTileFound = true;

                        // Sets the wall texture to use
                        columns[rayI].setTextureNumber(map.getMap()[MapLoc.y][MapLoc.x]);
                    }

                }
            }

            Vector2f intersection(rayStart + rayDir * distance);

            float T = 1;

            rays[rayI].setHead(rayStart.x, rayStart.y);
            rays[rayI].setTail(intersection.x, intersection.y);


            if (sideHit == 1)
                T = std::fmod(intersection.y, STEP) / STEP;
            if (sideHit == 0)
                T = std::fmod(intersection.x, STEP) / STEP;


            columns[rayI].CC(distance, rayAngle, T);

            rayAngle += increment;
            rayI++;
        }



        // Drawing everything
        window.draw(playerShape);

        // draw the 2d walls
        for (const auto& wall : walls) {
            window.draw(&wall[0], 2, sf::Lines);
        }

        // draw the rays
        for (int i = 0; i < rayCount; i++) {
            window.draw(rays[i].getLine(), 2, Lines);
        }

        // draw the textures on the 3d walls
        for (int i = 0; i < rayCount; i++) {
            Wall currentWall = columns[i];


            sf::Sprite wallSPRITE = Sprites[currentWall.getTextureNumber()][int(currentWall.getColumnToTexture())];

            wallSPRITE.setScale(10.f, currentWall.getHeight() / 64.f);
            wallSPRITE.setPosition(currentWall.getWall().getPosition().x, WindowTwoHeight / 2 - (currentWall.getHeight() / 2));

            window2.draw(wallSPRITE);
        }


        // drawing a random brick on window1
        for (int i = 0; i < 64; i++) {
            window.draw(Sprites[1][i]);
        }



        window.display();
        window2.display();


    }



    return 0;
}

