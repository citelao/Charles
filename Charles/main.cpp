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

int main(int argc, const char * argv[])
{
    std::cout << "Go, Charles!\n";
    
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
            Color c = cast(*new Point3D(cx, cy, 0), *new Vector3D());
            
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
    // (x-200)^2+(y-200)^2+(z-200)^2 == (100)^2
    
    if (pow(_pos.x - 200, 2) + pow(_pos.y - 200, 2) <= pow(100, 2)) { // Ray collides with sphere.
        
        unsigned char g = (unsigned char) 300 - _pos.y;
        return {255, g, 255, 255};
    }
    
    return {0, 0, 0, 255};
}