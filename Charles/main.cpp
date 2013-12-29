//
//  main.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/23/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "main.h"

/**
 * BEGIN THE PROGRAM
 **/
int main(int argc, const char * argv[])
{
    std::cout << "Go, Charles!\n";
    
    // Make spheres. Lots of spheres.
//    double _r = 40;
//    for (double _zp =  80; _zp <= 680; _zp += 150) {
//        for (double _xp = -400; _xp <= 400; _xp += 150) {
//            for (double _yp = -400; _yp <= 400; _yp += 150) {
//                double _i = 0;
//                objects.push_back(new Sphere(_xp + _i, _yp + _i, _zp + _i, _r));
//            }
//        }
//    }
//    objects.push_back(new Sphere(0, 0, 160, 20));
//    objects.push_back(new Sphere(0, 0, 200, 80));
    
    objects.push_back(new Sphere(-200, 40, 0, 120));
    objects.push_back(new Sphere(-40, 0, 0, 20));
    objects.push_back(new Sphere(-80, 0, 0, 20));
    
    // Quicksort z order.
    // TODO
    
    // Light 'em up.
    lights.push_back(Light(0, 0, -30, 10));
    
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
    
    std::thread rt1(&render);
    std::thread rt2(&render);
    std::thread rt3(&render);
    std::thread rt4(&render);
    rt1.detach();
    rt2.detach();
    rt3.detach();
    rt4.detach();
    
    /**
     * Sensible optimization
     **/
    window.setFramerateLimit(60);
    
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
        
        if(currentState == notifying) {
            std::cout << "Done Rendering \n";
            std::cout << "Collided rays: " << collided << "\n";
            std::cout << "Shadow checks: " << checks << "\n";
            
            currentState = done;
        }
        else if (currentState == rendering)
        {
            if (renderedPoints >= w*h) {
                currentState = notifying;
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
    }
    
    return 0;
}

void render()
{
    std::cout << "Render thread started! \n";
    
    double szz = screenPos.z - camera.z;
    double szx = screenPos.x - camera.x;
    double szy = screenPos.y - camera.y;
    
    Vector3D scv = screenPos - camera;
    
    while (currentState == rendering) {
        // Get an unrendered point
        Point2D p2d = getNextPoint();
        
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
          pow(szz, 2)
          - pow(szx + p2d.x, 2)
          - pow(szy + p2d.y, 2)
        );
        
        // Create a vector from origin to spherical screenspace and make it a unit vector.
        Vector3D uv = (scv + Vector3D(p2d.x, p2d.y, sphereZ)).unit();
        Point3D p = Point3D(screenPos.x + p2d.x, screenPos.y + p2d.y, screenPos.z);
        
        Ray3D r = Ray3D(p, uv);
        
        // Cast a ray from the screen point in the newly calculated direction.
        Color c = cast(r);
        
        // Write the RGBA codes to the unsigned char array.
        renderImage[((p2d.y * w) + p2d.x) * 4]     = c.r;
        renderImage[((p2d.y * w) + p2d.x) * 4 + 1] = c.g;
        renderImage[((p2d.y * w) + p2d.x) * 4 + 2] = c.b;
        renderImage[((p2d.y * w) + p2d.x) * 4 + 3] = c.a;
        
        // Increment total number of rendered points.
        renderedPoints++;
    }

    std::cout << "Render thread completed! \n";
}

Point2D getNextPoint()
{
    if (renderedPoints >= w * h * 4 / 5) {
        for (int i = 0; i <= w * h; i++) {
            if (renderImage[i * 4 + 3] == '\0') {
                int y = i / w;
                int x = i - y * w;
                
                return Point2D{x, y};
            }
        }
    }
    
    int position;
    bool ptRendered = true;
    
    while (ptRendered) {
        position = (random() % (w * h));
        unsigned char point = renderImage[position * 4 + 3];
        
        if (point == '\0')
            ptRendered = false;
    }
    
    int y = position / w;
    int x = position - y * w;
    
    return Point2D{x, y};
}

/**
 * Cast a ray from a _point with direction _versor
 **/
Color cast(const Ray3D &_r, int _bounces)
{
    if (_bounces >= 3) {
        return Color{0, 0, 0, 255};
    }
    
    for (int i = 0; i < objects.size(); i++) {
        // TODO legit color calculation.
        PhysicalObject* _object = objects[i];

        Point3D collision;
        bool collides = _object->collides(_r, &collision);
        
        if (collides == true) { // Ray collides with sphere.
            
            collided++;
            
            Vector3D normal = _object->normal(collision);
            
            // Send out a light ray
            Vector3D light = collision - lights[0].center;
            double cross = light * normal;
            
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
            
            // Send out a shadow ray
            double shadow = 1;
            for (int j = 0; j < objects.size(); j++) {
                if(j==i) {
                    continue;
                }
                
                PhysicalObject* _pblocker = objects[j];
                Point3D contacts;
                
                if (_pblocker->collides(Ray3D(collision, light), &contacts)) {
                    shadow = 0;
                }
                
                checks++;
            }
            
            double b = 255 * rangeness * fluxness * shadow;
            
            if ( b > 255) {
                b = 255;
            }
            
            return Color{(unsigned char) b, (unsigned char) (b * 130 / 255), (unsigned char) b, 255};
        }
    }
    
    return Color{0, 0, 0, 255};
}