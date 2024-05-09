#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "ray.h"
#include <cmath>
#include <vector>
#include <array>
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



    std::vector<std::array<Vertex, 2>> walls;






    ray rays[360];

    for (int i = 0; i < 360; i++) {
        rays[i].setAngle((float)i);
    }

    Map map;
    map.createMap(walls);



    float rayLength = 1000.f;


    CircleShape playerShape(10.f);
    float playerOffset = 10.f;

    Vector2f playerLoc(windowSize[0] / 2, windowSize[1] / 2);


    Player player(windowSize[0] / 2, windowSize[1] / 2);
    player.setSpeed(1.f);


    


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

        Vector2f currentLocXY = player.getLocation();
        int currentGridLocX = (currentLocXY.x + 10.f) / 64;
        int currentGridLocY = (currentLocXY.y + 10.f) / 64;


        //player.handleLocs(Vector2f(currentGridLocX, currentGridLocY));

        //player.collide(map.getMap());
        


        while (window2.pollEvent(event2))
        {
            if (event2.type == Event::Closed)
                window2.close();
        }

        // Clear
        window.clear();
        window2.clear();





        playerShape.setPosition(Vector2f(player.getDX(), player.getDY()));
        


        for (int i = 0; i < FOV; i++) {
            float tempAngle = i + player.getAngle();
            if (tempAngle >= 360) tempAngle -= 360;
            if (tempAngle < 0) tempAngle += 360;

            rays[i].setHead(playerShape.getPosition().x + playerOffset, playerShape.getPosition().y + playerOffset);
            rays[i].setTail(playerShape.getPosition().x + cos(tempAngle * M_PI / 180) * rayLength,
                playerShape.getPosition().y + sin(tempAngle * M_PI / 180) * rayLength);

            float closestDistance = std::numeric_limits<float>::max();
            float closestx = 0.0f;
            float closesty = 0.0f;

            for (int j = 0; j < walls.size(); j++) {
                float* tu = lookAtWall(walls[j], rays[i].getLine()[0].position, rays[i].getLine()[1].position);
                float t = tu[0];
                float u = tu[1];

                if (t > 0 && t < 1 && u > 0) {
                    float x1 = walls[j][0].position.x;
                    float y1 = walls[j][0].position.y;
                    float x2 = walls[j][1].position.x;
                    float y2 = walls[j][1].position.y;

                    float hitpointx = x1 + t * (x2 - x1);
                    float hitpointy = y1 + t * (y2 - y1);

                    float distance = std::sqrt((hitpointx - rays[i].getLine()[0].position.x) * (hitpointx - rays[i].getLine()[0].position.x) +
                        (hitpointy - rays[i].getLine()[0].position.y) * (hitpointy - rays[i].getLine()[0].position.y));

                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestx = hitpointx;
                        closesty = hitpointy;
                    }
                }
            }

            if (closestDistance != std::numeric_limits<float>::max()) {
                rays[i].setTail(closestx, closesty);
            }


            Wall wall(closestDistance, i);
            window2.draw(wall.getWall());



        }




        // Color
        //circle.setFillColor(Color(255, 145, 100, 150));
        //rect.setFillColor(Color::Blue);


        // Update
        //circle.move(0.1f, 0.0f);
        /*rect.rotate(5.f);*/


        // Draw
        window.draw(playerShape);

        
        for (const auto& wall : walls) {
            window.draw(&wall[0], 2, sf::Lines);
        }


        for (int i = 0; i < FOV; i++) {
            window.draw(rays[i].getLine(), 2, Lines);
        }


        window.display();
        window2.display();


    }
    return 0;
}

