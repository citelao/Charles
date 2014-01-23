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
    
    start = time(NULL);
    
    // Make spheres. Lots of spheres.
//    double _r = 40;
//    for (double _zp = -200; _zp < 600; _zp += 100) {
//    for (double _xp = -200; _xp <= 200; _xp += 100) {
//    for (double _yp = -10; _yp <= 200; _yp += 100) {
//        objects.push_back(new Sphere(_xp, _yp, _zp, _r, 0));
//    }
//    }
//    }
    
    objects.push_back(new RectPrism(-700, -20, -900, 9001, 10, 9001, 0.1));
    
    objects.push_back(new Sphere(-100, 30, 300, 40, 0.9));
    objects.push_back(new Sphere(-180, 30, 270, 40, 0.9));
    objects.push_back(new Sphere(-80, 30, 200, 40, 0));
    
//    objects.push_back(new RectPrism(50, -10, 150, 10, 200, 300, 0.9));

    // Light 'em up.
//    lights.push_back(Light(0, 300, 300, 120));
//    lights.push_back(Light(300, 0, 300, 200));
    lights.push_back(Light(100, 100, 300, 80));
    lights.push_back(Light(-100, 100, 350, 80));
    lights.push_back(Light(-200, 100, 50, 80));
    
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
            end = time(NULL);
            
            double secs = (double)(end - start);
            
            std::cout << "Done Rendering \n";
            std::cout << "Render time: " << secs << " seconds \n";
            std::cout << "Total Points Rendered: " << totalRenderedPoints << "/" << totalPixels << "\n";
            std::cout << "Collided rays: " << collided << "\n";
            std::cout << "Shadow checks: " << checks << "\n";
            
            currentState = state::done;
        }
        else if (currentState == state::rendering) // Update screen if rendering
        {
            // If we're done rendering everything, stop the rendering. Pretty self explanatory.
            // This code is dumb and compensates for a poorly written counter.
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
        // Thanks Morgan and Thomas Redding!
        Vector3D cZPrime = eye.uv;
        Vector3D cXPrime = Vector3D(cZPrime.z, 0, -cZPrime.x);
        Vector3D cYPrime = cZPrime.cross(cXPrime);
        
        Vector3D p = (cXPrime * offset.x) +
                     (cYPrime * offset.y) +
                     (cZPrime * offset.z);
        
        Vector3D uv = ((eye.traverse(screenDistance).p + p) - eye.p).unitize();
        
        // Start from eye
        Ray3D r = Ray3D(eye.p, uv);
        
        // Cast a ray from the screen point in the newly calculated direction.
        Color c = cast(r);
        
        if (debug == mode::onscreen && ((screenPoint.x == 0 && screenPoint.y == 0) ||
            (screenPoint.x == w/2 && screenPoint.y == h/2) ||
            (screenPoint.x == w-1 && screenPoint.y == h-1))) {
            c = Color(255, 255, 255);
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
    
    return Point2D(x, y);
}

/**
 * Cast a ray. Recursive!
 **/
Color cast(const Ray3D &_r, int _bounces)
{
    // Find the closest object.
    PhysicalObject* closest = NULL;
    Point3D collision;
    double closestDistance = INFINITY;
    for (int i = 0; i < objects.size(); i++) { // θ(n)
        PhysicalObject* _object = objects[i];
        
        Point3D _collision;
        bool collides = _object->collides(_r, &_collision);
        
        if (collides) { // Ray collides with object.
            double distance = (_r.p - _collision).squaredmagnitude();
            
            if (distance < closestDistance) {
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
            return Color(255, 255, 255);
        }
        
        Vector3D normal = closest->normal(collision);
        
        if (debug == mode::normal) {
            return Color(126 + 126 * normal.x, 126 + 126 * normal.y, 126 + 126 * normal.z);
        }
        
        if (debug == mode::normalz) {
            unsigned char b = 126-126 * normal.z;
            
            return Color(b,b,b);
        }
        
        // Send out a reflection ray
        double reflectivity = closest->reflectivity(collision);
        
        Color reflection = Color(0, 0, 0);
        if (reflectivity >= 0 + tolerance) {
            if (_bounces < maxBounces) {
                reflection = cast(Ray3D(collision, _r.uv.reflect(normal)), _bounces + 1);
            }
        }
        
        // Send out a refraction ray
        // cast(contactpt, _uv * diff, _bounces + 1);
        
        // Send out light rays to each light.
        // Using Lambertian reflectance for now.
        Color localColor = closest->color(collision);
        double brightness = 0;
        
        if (reflectivity <= 1 - tolerance) {
            for (int i = 0; i < lights.size(); i++) {
                Vector3D light = lights[i].center - collision;
                
                if (debug == mode::lightz) {
                    unsigned char b = 126-126 * light.unitize().z;
                    
                    return Color(b,b,b);
                }
                
                // Make sure this light is not obscured.
                double shadow = 1;
                for (int j = 0; j < objects.size(); j++) {
                    PhysicalObject* _pblocker = objects[j];
                    Point3D contacts;
                    
                    if (_pblocker == closest) {
                        continue;
                    }
                    
                    if (_pblocker->collides(Ray3D(collision, light), &contacts)) {
                        if ((contacts - collision).magnitude() > (light).magnitude()) { // If object is *behind* light.
                            continue;
                        }
                        
                        if (debug == mode::shadows) {
                            return Color(255,0,0);
                        }
                        
                        shadow = 0;
                        break;
                    }
                }
                
                if (shadow == 0) {
                    continue;
                }
                
                double lambertian = (normal * light) / pow((light).magnitude(), 2);
                
                if (lambertian < 0) {
                    lambertian = 0;
                }
                
                brightness += lights[i].intensity * lambertian;
                
                if ( brightness >= 1) {
                    brightness = 1;
                    break;
                }
            }
        }
        
        Color lambertian = localColor * brightness;
        
        return reflection * reflectivity + lambertian * (1-reflectivity);
    }
    
    return Color(0, 0, 0);
}