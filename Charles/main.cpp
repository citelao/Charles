//
//  main.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/23/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

// refs:
// - http://en.sfml-dev.org/forums/index.php?topic=3543.0 (uint8 and image class)

#include <iostream>
#include <string>
#include <thread>
#include <math.h>
#include <SFML/graphics.hpp>
#include "Vector3D.h"
#include "Point3D.h"

struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
};

void render(int _x, int _y, int _w, int _h);
Color cast(Point3D _pos, Vector3D _uvec);

int w = 400;
int h = 400;
unsigned char *renderImage = new unsigned char[w * h * 4];

Point3D screenPos(- w / 2, - h / 2, 0);
Point3D camera(0, 0, -0.3);

int main(int argc, const char * argv[])
{
    std::cout << "Go, Charles!\n";
    
    std::cout << "\nscreenPos: " << screenPos.x << ", ";
    std::cout << screenPos.y << ", ";
    std::cout << screenPos.z << ".\n";
    
    // Create window.
    sf::RenderWindow window(sf::VideoMode(w, h), "Charles");
    
    // Instantiate texture and sprite.
    sf::Texture renderTexture;
    sf::Sprite renderSprite;
    
    renderTexture.create(w, h);
    
    // Start render threads.
    std::thread rt1(&render, 0, 0, w/2, h/2);
    std::thread rt2(&render, w/2, 0, w/2, h/2);
    std::thread rt3(&render, 0, h/2, w/2, h/2);
    std::thread rt4(&render, w/2, h/2, w/2, h/2);
    rt1.detach();
    rt2.detach();
    rt3.detach();
    rt4.detach();
    
    while(window.isOpen())
    {
        // Check for exit command.
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Clear screen.
        window.clear(sf::Color::Black);
        
        // Set texture to unsigned char array rendered to.
        renderTexture.update(renderImage);
        renderSprite.setTexture(renderTexture);
        
        // Display the frame
        window.draw(renderSprite);
        window.display();
    }
    
    return 0;
}

void render(int _x, int _y, int _w, int _h)
{
    std::cout << "Render thread started! \n";
    
    for (int cy = _y; cy < _y + _h; cy++) {
        for (int cx = _x; cx < _x + _w; cx++ ) {
            Vector3D v = screenPos - camera + Vector3D(cx, cy, 0);
            Vector3D uv = v / v.magnitude();
            
            Color c = cast(*new Point3D(screenPos.x + cx, screenPos.y + cy, screenPos.z), uv);
            
            renderImage[((cy * w) + cx) * 4]     = c.r;
            renderImage[((cy * w) + cx) * 4 + 1] = c.g;
            renderImage[((cy * w) + cx) * 4 + 2] = c.b;
            renderImage[((cy * w) + cx) * 4 + 3] = c.a;
        }
    }
    
    std::cout << "Render thread completed! \n";
}

Color cast(Point3D _pos, Vector3D _uvec)
{
    
    // Let's create a sphere dead center of the screen.
    double xp = 0;
    double yp = 0;
    double zp = 2;
    double r = 2;

    // Use quadratics
    double a = pow(_uvec.x, 2) + pow(_uvec.y, 2) + pow(_uvec.z, 2);
    double b = 2 * ((_pos.x - xp) * _uvec.x + (_pos.y - yp) * _uvec.y + (_pos.z - zp) * _uvec.z);
    double c = pow(_pos.x - xp, 2) + pow(_pos.y - yp, 2) + pow(_pos.z - zp, 2) - pow(r, 2);
    
    double far = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double near = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    
    if(_pos.x >= -10) {
           std::string nf;
    }
    if (!isnan(near) && !isnan(far)) { // Ray collides with sphere.
        
//        std::cout << "near: " << near << "; far: " << far << std::endl;
        unsigned char g = (unsigned char) 255 - near;
        return {255, g, 255, 255};
    }
    
    return {0, 0, 0, 255};
}