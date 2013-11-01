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

int w = 1200;
int h = 800;
unsigned char *renderImage = new unsigned char[w * h * 4];

Point3D screenPos(- w / 2, - h / 2, 0);
Point3D camera(0, 0, - 450);

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
    
    // For each row from starting row to height,
    // scan through each column from starting col to width.
    for (int cy = _y; cy < _y + _h; cy++) {
        for (int cx = _x; cx < _x + _w; cx++ ) {
            /****
             * Construct unit vector pointing from eye to screen pixel as sphere. 
             ****
             * Screen is stored mathematically as top-left corner of screen rectangle, which
             * needs to be transformed into a sphere. See math projection.
             **/
            
            // Use the 3D Pythagorean theorem: h**2 = x**2 + y**2 + z**2
            // or (since we know the hypotenuse), sqrt(h**2 - x**2 - y**2) = z
            double sphereZ = sqrt(
                pow(screenPos.z - camera.z, 2)
              - pow(screenPos.x - camera.x + cx, 2)
              - pow(screenPos.y - camera.y + cy, 2)
            );
            
            // Create a vector from origin to spherical screenspace.
            Vector3D v = screenPos - camera + Vector3D(cx, cy, sphereZ);
            // And make it a unit vector.
            Vector3D uv = v / v.magnitude();
            
            /**
             * Cast a ray from the screen point in the newly calculated direction.
             **/
            Color c = cast(*new Point3D(screenPos.x + cx, screenPos.y + cy, screenPos.z), uv);
            
            /**
             * Write the RGBA codes to the unsigned char array
             **/
            renderImage[((cy * w) + cx) * 4]     = c.r;
            renderImage[((cy * w) + cx) * 4 + 1] = c.g;
            renderImage[((cy * w) + cx) * 4 + 2] = c.b;
            renderImage[((cy * w) + cx) * 4 + 3] = c.a;
        }
    }
    
    std::cout << "Render thread completed! \n";
}

Color cast(Point3D _p, Vector3D _uv)
{
    
    // Let's create a sphere dead center of the screen.
    double xp = 200;
    double r = 50;
    
    for (double zp = 100; zp <= 3000; zp += 800) {
    for (double yp = -600; yp <= 1000; yp += 300) {
        // Use the quadratic equation to solve for sphere intersection.
        double a = pow(_uv.x, 2) + pow(_uv.y, 2) + pow(_uv.z, 2);
        double b = 2 * ((_p.x - xp) * _uv.x + (_p.y - yp) * _uv.y + (_p.z - zp) * _uv.z);
        double c = pow(_p.x - xp, 2) + pow(_p.y - yp, 2) + pow(_p.z - zp, 2) - pow(r, 2);
        
        double far = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        double near = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        
        if (!isnan(near) && !isnan(far)) { // Ray collides with sphere.
            
            unsigned char g = (unsigned char) 0;
            return {255, g, (unsigned char) zp, 255};
        }
    }
    }
    
    return {0, 0, 0, 255};
}