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
#include <vector>
#include "Vector3D.h"
#include "Point3D.h"
#include "Sphere.h"
#include "Light.h"

/**
 * The ever-useful color struct
 **/
struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
};

/**
 * Define them variables.
 **/
bool debug = false;

int w = 512;
int h = 512;
unsigned char *renderImage = new unsigned char[w * h * 4];

Point3D screenPos(- w / 2, - h / 2, 0); // TODO, since we use projection now.
Point3D camera(0, 0, - 500);

std::vector<Sphere> spheres {};
std::vector<Light> lights {};

/**
 * Function time!
 **/
void render(int _x, int _y, int _w, int _h);
Color cast(Point3D _pos, Vector3D _uvec, int _bounces = 0);

/**
 * BEGIN THE PROGRAM
 **/
int main(int argc, const char * argv[])
{
    std::cout << "Go, Charles!\n";
    
    // Make spheres. Lots of spheres.
    double _r = 40;
    for (double _zp =  100; _zp <= 1600; _zp += 300) {
        for (double _xp = -400; _xp <= 400; _xp += 200) {
            for (double _yp = -400; _yp <= 400; _yp += 100) {
                // double _i = rand() % 400 - 200;
                double _i = 0;
                spheres.push_back(Sphere(_xp + _i, _yp + _i, _zp + _i, _r));
            }
        }
    }
    
    // Quicksort z order.
    // TODO
    
    // Light 'em up.
    lights.push_back(Light(-150, 100, 200, 10));
    
    // Create window
    sf::RenderWindow window(sf::VideoMode(w, h), "Charles");
    
    // Instantiate texture and sprite.
    sf::Texture texture;
    sf::Sprite sprite;
    texture.create(w, h);
    
    /** 
     * Start render threads.
     * One for each quarter of the screen.
     **/
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
        texture.update(renderImage);
        sprite.setTexture(texture);
        
        // Display the frame
        window.draw(sprite);
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
             * See:
             * - http://en.wikipedia.org/wiki/Curvilinear_perspective
             * - http://en.wikipedia.org/wiki/3D_projection
             * - http://en.wikipedia.org/wiki/Camera_matrix
             * - and the book
             **/
            
            // Use the 3D Pythagorean theorem: h**2 = x**2 + y**2 + z**2
            // or (since we know the hypotenuse), sqrt(h**2 - x**2 - y**2) = z
            double sphereZ = sqrt(
                pow(screenPos.z - camera.z, 2)
              - pow(screenPos.x - camera.x + cx, 2)
              - pow(screenPos.y - camera.y + cy, 2)
            );
            
            // Create a vector from origin to spherical screenspace and make it a unit vector.
            Vector3D uv = (screenPos - camera + Vector3D(cx, cy, sphereZ)).unit();
            
            // Cast a ray from the screen point in the newly calculated direction.
            Color c = cast(*new Point3D(screenPos.x + cx, screenPos.y + cy, screenPos.z), uv);
            
            // Write the RGBA codes to the unsigned char array.
            renderImage[((cy * w) + cx) * 4]     = c.r;
            renderImage[((cy * w) + cx) * 4 + 1] = c.g;
            renderImage[((cy * w) + cx) * 4 + 2] = c.b;
            renderImage[((cy * w) + cx) * 4 + 3] = c.a;
        }
    }
    
    std::cout << "Render thread completed! \n";
}

/**
 * Cast a ray from a _point with direction _versor
 **/
Color cast(Point3D _point, Vector3D _uv, int _bounces)
{
    if (_bounces >= 3) {
        return Color{0, 0, 0, 255};
    }
    
    for (int i = 0; i <= spheres.size(); i++) {
        Sphere _sphere = spheres[i];
        
        // Use the quadratic equation to solve for sphere intersection.
        double a = pow(_uv.x, 2) + pow(_uv.y, 2) + pow(_uv.z, 2);
        double b = 2 * ((_point.x - _sphere.center.x) * _uv.x + (_point.y - _sphere.center.y) * _uv.y + (_point.z - _sphere.center.z) * _uv.z);
        double c = pow(_point.x - _sphere.center.x, 2) + pow(_point.y - _sphere.center.y, 2) + pow(_point.z - _sphere.center.z, 2) - pow(_sphere.radius, 2);
        
        double far = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        double near = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        
        if (!isnan(near) && !isnan(far)) { // Ray collides with sphere.
            
            // Calcuate collision point & normal
            Point3D collision = _point + _uv * near;
            Vector3D normal = collision - _sphere.center;
            
            // Send out a light ray
            Vector3D light = collision - lights[0].center;
            double cross = light * normal;
            
            // Send out a shadow ray
            // Send out a reflection ray
            // cast(contactpt, normal, _bounces + 1);
            // Send out a refraction ray
            // cast(contactpt, _uv * diff, _bounces + 1);
            
            // Color!
            double rangeness = 1 / sqrt(light.magnitude()) * lights[0].intensity;
            
            if (rangeness <= 0) {
                unsigned char g = (unsigned char) (debug) ? 255 : 0;
                return Color{0, g, 0, 255};
            }
            
            double fluxness = - cross / (light.magnitude() * normal.magnitude());
            
            if (fluxness <= 0) {
                unsigned char r = (unsigned char) (debug) ? 255 : 0;
                return Color{r, 0, 0, 255};
            }
            
            double b = rangeness * fluxness * 255;
            
            if ( b > 255) {
                b = 255;
            }
//            } else {
                return Color{(unsigned char) b, (unsigned char) 0, (unsigned char) b, 255};
//            }
        }
    }
    
    return Color{0, 0, 0, 255};
}