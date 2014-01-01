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
    double _r = 40;
    for (double _zp = -200; _zp <= 0; _zp += 100) {
    for (double _xp = -200; _xp <= 200; _xp += 100) {
    for (double _yp = -200; _yp <= 200; _yp += 100) {
        objects.push_back(new Sphere(_xp, _yp, _zp, _r));
    }
    }
    }
    
    // Light 'em up.
    lights.push_back(Light(0, 0, -400, 10));
    
    // Create window
    sf::RenderWindow window(sf::VideoMode(w, h), "Charles");
    
    // Instantiate texture and sprite.
    sf::Texture texture;
    sf::Sprite sprite;
    texture.create(w, h);
    
    /** 
     * Start render threads.
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
        /**
         * SFML stuff
         **/
        
        // Check for exit command.
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        /**
         * Leader thread debug & optim stuff
         **/
        
        // Show unrendered points in the correct debug mode
        if (debug == mode::unrendered) {
            for (int i = 0; i < totalPixels; i++) {
                if (renderedPoints[i] == false) {
                    renderImage[i * 4]   = 255; // r
                    renderImage[i * 4+1] =   0; // g
                    renderImage[i * 4+2] = 255; // b
                    renderImage[i * 4+3] = 255; // a
                }
            }
        }
        
        /**
         * Display stuff
         **/
        
        // Determine correct action based on state
        if (currentState == state::notifying) // Print out statistics once if done rendering
        {
            std::cout << "Done Rendering \n";
            std::cout << "Total Points Rendered: " << totalRenderedPoints << "/" << totalPixels << "\n";
            std::cout << "Collided rays: " << collided << "\n";
            std::cout << "Shadow checks: " << checks << "\n";
            
            currentState = state::done;
        }
        else if (currentState == state::rendering) // Update screen if rendering
        {
            // If we're done rendering everything, stop the rendering. Pretty self explanatory.
            bool done = true;
            for (int i = 0; i < totalPixels; i++) {
                if (!renderedPoints[i]) {
                    done = false;
                }
            }
            
            if (done)
            {
                currentState = state::notifying;
            }
            
            std::cout << totalRenderedPoints << "/" << totalPixels << " " << totalPixels-totalRenderedPoints << "\n";
            
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
    
    // TODO rewrite to use frustrum
    double szz = screenPos.z - camera.z;
    double szx = screenPos.x - camera.x;
    double szy = screenPos.y - camera.y;
    
    Vector3D scv = screenPos - camera;
    
    while (currentState == state::rendering) {
        // Get an unrendered point
        Point2D screenPoint = getNextPoint();
        
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
          - pow(szx + screenPoint.x, 2)
          - pow(szy + screenPoint.y, 2)
        );
        
        // Create a vector from origin to spherical screenspace and make it a unit vector.
        Vector3D uv = (scv + Vector3D(screenPoint.x, screenPoint.y, sphereZ)).unit();
        Point3D p = Point3D(screenPos.x + screenPoint.x, screenPos.y + screenPoint.y, screenPos.z);
        
        Ray3D r = Ray3D(p, uv);
        
        // Cast a ray from the screen point in the newly calculated direction.
        Color c = cast(r);
        
        // Write the RGBA codes to the unsigned char array.
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4]     = c.r;
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4 + 1] = c.g;
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4 + 2] = c.b;
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4 + 3] = 255; // Alpha must always be 255 to show.
        
        // Increment total number of rendered points.
        totalRenderedPoints++;
    }

    std::cout << "Render thread completed! \n";
}

Point2D getNextPoint()
{
    // TODO allocate points to render on launch to avoid thread-related gaps.
    // I think right now ~200 points are accidentally doubled.
    
//    // If most points are rendered, switch to sweeping.
//    if (totalRenderedPoints >= totalPixels * .95) {
//        for (int i = 0; i < totalPixels; i++) {
//            if (!renderedPoints[i]) {
//                int y = i / w;
//                int x = i - y * w;
//                
//                renderedPoints[i] = true;
//                
//                return Point2D{x, y};
//            }
//        }
//    }
    
    int position;
    bool ptRendered = true;
    
    while (ptRendered && currentState == state::rendering) {
        position = arc4random() % totalPixels;
        
        if (renderedPoints[position] == false) {
            renderedPoints[position] = true;
            ptRendered = false;
        }
        
    }
    
    renderedPoints[position] = true;
    
    int y = position / w;
    int x = position - y * w;
    
    return Point2D{x, y};
}

/**
 * Cast a ray. Not yet recursive!
 **/
Color cast(const Ray3D &_r, int _bounces)
{
    if (_bounces >= 3) {
        return Color{0, 0, 0, 255};
    }
    
    int closest = 0;
    float closestDistance;
    for (int i = 1; i < objects.size(); i++) {
        PhysicalObject* _object = objects[i];
        
        
    }
    
    for (int i = 0; i < objects.size(); i++) {
        // TODO closest object, not first
        // TODO legit color calculation.
        PhysicalObject* _object = objects[i];

        Point3D collision;
        bool collides = _object->collides(_r, &collision);
        
        if (collides == true) { // Ray collides with sphere.
            
            if (debug == mode::onscreen) {
                return Color{255, 255, 255, 255};
            }
            
            collided++;
            
            // TODO why isn't this unitized
            Vector3D normal = _object->normal(collision);
            
            if (debug == mode::normal) {
                Vector3D un = normal.unit();
                return Color{(unsigned char)(126 + 126 * un.x), (unsigned char)(126 + 126 * un.y), (unsigned char)(126 + 126 * un.z), 255};
            }
            
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
                unsigned char g = (unsigned char) (debug == mode::light) ? 255 : 0;
                return Color{0, g, 0, 255};
            }
            
            double fluxness = - cross / (light.magnitude() * normal.magnitude());
            
            if (fluxness <= 0) {
                unsigned char r = (unsigned char) (debug == mode::light) ? 255 : 0;
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
                    if (debug == mode::shadows) {
                        return Color{255,0,0,255};
                    }
                    
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