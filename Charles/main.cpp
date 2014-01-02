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
    for (double _zp = 50; _zp < 600; _zp += 100) {
    for (double _xp = -200; _xp <= 200; _xp += 100) {
    for (double _yp = -200; _yp <= 200; _yp += 100) {
        objects.push_back(new Sphere(_xp, _yp, _zp, _r));
    }
    }
    }
    
    // Light 'em up.
    lights.push_back(Light(0, 0, 200, 10));
    
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
            
            //  std::cout << totalRenderedPoints << "/" << totalPixels << " " << totalPixels-totalRenderedPoints << "\n";
            
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
    
    // w/(2tan(fov/2))
    double screenDistance = w / pixelsPerMeter / 2 / tan(M_PI * fov / 2 / 180);
    
    while (currentState == state::rendering) {
        // Get an unrendered point & convert its onscreen position to 2D deviation from eye ray.
        Point2D screenPoint = getNextPoint();
        Point2D offsetPoint = Point2D{screenPoint.x - w / 2, h / 2 - screenPoint.y };
        
        Vector3D offset = Vector3D(offsetPoint.x / pixelsPerMeter, offsetPoint.y / pixelsPerMeter, 0);

        // Create a coordinate system relative to eye ray.
        // Z' is eye's uv.
        // X' is parallel to X-Z plane, at least for now. Arbitrary rotation to come later. (TODO)
        // X' is also perpendicular to Z', the eye's vector. Their dot product is 0.
        // Y' should be Z' cross X'.
        Vector3D cZPrime = eye.uv;
        Vector3D cXPrime = Vector3D(0, 1, 0).cross(cZPrime); // TODO ??
        Vector3D cYPrime = cZPrime.cross(cXPrime);
        
        Ray3D cPrime = Ray3D(eye.traverse(screenDistance).p,
                             cZPrime + cXPrime + cYPrime);
        
        // Combine offset and coordinate system.
        Vector3D cPrimeOffset = Vector3D(cPrime.uv.x * offset.x,
                                         cPrime.uv.y * offset.y,
                                         cPrime.uv.z * offset.z);

        Point3D p = cPrime.p + cPrimeOffset;
        Vector3D uv = (p - eye.p).unitize();
        
        // Parallel projection!
        // Vector3D uv = Vector3D(0,0,-1);
        
        Ray3D r = Ray3D(p, uv);
        
        // Cast a ray from the screen point in the newly calculated direction.
        Color c = cast(r);
        
        if (debug == mode::onscreen && ((screenPoint.x == 0 && screenPoint.y == 0) ||
            (screenPoint.x == 256 && screenPoint.y == 256) ||
            (screenPoint.x == 511 && screenPoint.y == 511))) {
            c = Color{255,255,255,255};
        }
        
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
    
    // Find the closest object.
    PhysicalObject* closest = NULL;
    Point3D collision;
    double closestDistance;
    for (int i = 0; i < objects.size(); i++) {
        PhysicalObject* _object = objects[i];
        
        Point3D _collision;
        bool collides = _object->collides(_r, &_collision);
        
        if (collides) { // Ray collides with object.
            double distance = (_r.p - _collision).magnitude();
            
            if (distance < closestDistance || closest == NULL) {
                closestDistance = distance;
                collision = _collision;
                closest = _object;
            }
        }
    }
    
    if (closest != NULL) { // If there was a collision.
        // Increment number of collided rays for stat keeping.
        collided++;
        
        if (debug == mode::onscreen) {
            return Color{255, 255, 255, 255};
        }
        
        // TODO why isn't this unitized
        Vector3D normal = closest->normal(collision);
        
        if (debug == mode::normal) {
            Vector3D un = normal.unitize();
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
            PhysicalObject* _pblocker = objects[j];
            Point3D contacts;
            
            if(_pblocker == closest) {
                continue;
            }
            
            if (_pblocker->collides(Ray3D(collision, light), &contacts)) {
                if (debug == mode::shadows) {
                    return Color{255,0,0,255};
                }
                
                shadow = 0;
            }
            
            checks++;
        }
        
        // TODO this ain't how light works.
        double b = 255 * rangeness * fluxness * shadow;
        
        if ( b > 255) {
            b = 255;
        }
        
        return Color{(unsigned char) b, (unsigned char) (b * 130 / 255), (unsigned char) b, 255};
    }
    
    return Color{0, 0, 0, 255};
}