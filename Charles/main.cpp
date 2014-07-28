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

    // Time everything!
    start = time(NULL);
    
    /**
     * Instantiate scene
     **/
    
    // Make spheres. Lots of spheres.
//    double _r = 40;
//    for (double _zp = -200; _zp < 600; _zp += 100) {
//    for (double _xp = -200; _xp <= 200; _xp += 100) {
//    for (double _yp = -10; _yp <= 100; _yp += 100) {
//        objects.push_back(new Sphere(new SolidTexture(Color(255), 0.5), _xp, _yp, _zp, _r));
//    }
//    }
//    }
//    
    // Ground plane.
    objects.push_back(new RectPrism(new CheckerboardTexture(Color(255, 130, 0)), Point3D(-700, -20, -900), Vector3D(9001, 10, 9001)));
    
    objects.push_back(new Sphere(new SolidTexture(Color(255), 0, 1, 1.3), -100, 30, 300, 40));
//    objects.push_back(new Sphere(new SolidTexture(Color(255), 0.8, 1, 1.3), -180, 30, 270, 40));
    objects.push_back(new Sphere(new CheckerboardTexture(Color(255)), -180, 30, 270, 40));
//    objects.push_back(new Sphere(new SolidTexture(Color(255), 0, 1, 1), -80, 30, 200, 40));
    objects.push_back(new RectPrism(new SolidTexture(Color(255), 0, 1, 5), -60, -10, 200, 40, 40, 40));
    objects.push_back(new RectPrism(new CheckerboardTexture(Color(255), Color(0)), -300, -10, 400, 600, 500, 40));
    
    // Light 'em up.
    lights.push_back(Light(100, 100, 300, 80));
//    objects.push_back(new Sphere(new SolidTexture(Color(255), 0, 1, 1), 100, 100, 300, 40)); // light overlapper
    lights.push_back(Light(-100, 100, 350, 80));
    lights.push_back(Light(-200, 100, 50, 80));
    
    /**
     * SFML setup
     **/
    sf::Font font;
    
    if(!font.loadFromFile("mission.otf"))
        std::cerr << "whopo";
    
    sf::Text text("...", font);
    text.setCharacterSize(30);
    
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
            
            /**
             * Camera controls
             **/
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    dragMouseOrigin = Vector2D(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
                    dragEye = camera.eye;
                }
            }
            
            if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                        camera.eye.p = (dragEye.p - Vector3D(sf::Mouse::getPosition().x - dragMouseOrigin.x, dragMouseOrigin.y - sf::Mouse::getPosition().y, 0) * 0.5);
                    } else {
                        camera.eye.uv = (dragEye.uv - Vector3D(sf::Mouse::getPosition().x - dragMouseOrigin.x, dragMouseOrigin.y - sf::Mouse::getPosition().y, 0) * 0.005).unitize();
                    }
                
//                    std::cout << camera.eye.uv.x << " " << camera.eye.uv.y << "\n";
                    
                    unsigned char *tempRenderImage = renderImage;
                    renderImage = new unsigned char[totalPixels * 4] {255};
                    delete[] tempRenderImage;
                    
                    renderedPoints.reset();
                    
                    if (currentState != state::rendering) {
                        currentState = state::rendering;
                        std::thread rt1(&render);
                        std::thread rt2(&render);
                        std::thread rt3(&render);
                        std::thread rt4(&render);
                        rt1.detach();
                        rt2.detach();
                        rt3.detach();
                        rt4.detach();
                    }
                }
            }
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
                currentState = state::notifying;
            
            // Set texture to unsigned char array rendered to.
            texture.update(renderImage);
            sprite.setTexture(texture);
        }
        
        // Create a mouse cast line
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        mouseRay.clear();
        mouseRay.append(sf::Vector2f(mousePos.x, mousePos.y));
        bounces.clear();
        Ray3D r = camera.translate(Point2D(mousePos.x - w / 2, h / 2 - mousePos.y));
        cast(r, 0, NULL, &bounces);
        
        std::stringstream s;
        for (int i = 0; i < bounces.size(); i++) {
            Point2D p = camera.translate(bounces[i]);
            
            s << i << " x: " << bounces[i].x() << ", y: " << bounces[i].y() << ", z: " << bounces[i].z() << "\n";
            
            if (i == 0) {
                s << " px: " << p.x << ", y: " << p.y << "\n";
                
                Ray3D r2 = camera.translate(p).traverse(camera.screenDistance);
                s << "r2x: " << r2.p.x() << ", y: " << r2.p.y() << ", z: " << r2.p.z() << "\n";
            }
            
            mouseRay.append(sf::Vector2f(p.x + w / 2, - p.y + h / 2));
        }
        
        text.setString(s.str());
        
        // Clear screen.
        window.clear(sf::Color::Black);
        
        // Display the frame
        window.draw(sprite);
        window.draw(mouseRay);
        window.draw(text);
        window.display();

    }
    
    return 0;
}

void render()
{
    std::cout << "Render thread started! \n";
    
    while (currentState == state::rendering) {
        // Get an unrendered point & convert its onscreen position to 2D deviation from eye ray.
        Point2D screenPoint = getNextPoint();
        Point2D offsetPoint = Point2D{screenPoint.x - w / 2, h / 2 - screenPoint.y };
        
        Ray3D r = camera.translate(offsetPoint);
        
        // Cast a ray from the screen point in the newly calculated direction.
        Color c = cast(r);
        
        if (debug == mode::onscreen && ((screenPoint.x == 0 && screenPoint.y == 0) ||
            (screenPoint.x == w/2 && screenPoint.y == h/2) ||
            (screenPoint.x == w-1 && screenPoint.y == h-1))) {
            c = Color(255, 255, 255);
        }
        
        // Write the RGBA codes to the unsigned char array.
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4]     = c.r();
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4 + 1] = c.g();
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4 + 2] = c.b();
        renderImage[((screenPoint.y * w) + screenPoint.x) * 4 + 3] = c.a(); // Alpha must always be 255 to show.
        
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
Color cast(const Ray3D &_r, int _bounces, PhysicalObject *substance, std::vector<Point3D> *bounced)
{
    // Find the closest object.
    PhysicalObject* closest = NULL;
    Point3D collision;
    double closestT = INFINITY;
    for (int i = 0; i < objects.size(); i++) { // θ(n)
        PhysicalObject* _object = objects[i];
        
        Point3D _collision;
        double _t;
        bool collides = _object->collides(_r, &_collision, &_t);
        
        if (_t < 0 + tolerance)
            continue;
            
        if (collides) { // Ray collides with object.
            if (_t < closestT) {
                closestT = _t;
                collision = _collision;
                closest = _object;
            }
        }
    }
    
    if (closest == NULL)
        return Color(0);
    
    if (bounced != NULL) {
        bounced->push_back(collision);
    }
    
    // Increment number of collided rays for stat keeping.
    collided++;
    
    if (debug == mode::onscreen) {
        return Color(255);
    }
    
    Vector3D normal = closest->normal(collision);
    
    if (debug == mode::normal) {
        return Color(126 + 126 * normal.x(), 126 + 126 * normal.y(), 126 + 126 * normal.z());
    }
    
    if (debug == mode::normalz) {
        return Color(126-126 * normal.z());
    }
    
    // Send out a reflection ray
    double reflectivity = closest->texture->reflectivity(collision);
    Color reflection = Color(0);
    
    if (reflectivity >= 0 + tolerance) {
        if (_bounces < maxBounces) {
            reflection = cast(Ray3D(collision, _r.uv.reflect(normal)), _bounces + 1, substance, bounced);
        }
    }
    
    // Send out a refraction ray
    double refractivity = closest->texture->refractivity(collision);
    Color refraction = Color(0);
    
    if (refractivity >= 0 + tolerance) {
        if (_bounces < maxBounces) {
            double f;
            PhysicalObject *newSubst;
            if (substance == NULL) {
                f = 1.0 / closest->texture->indexOfRefraction(collision);
                newSubst = closest;
            } else {
                if (substance == closest) {
                    f = 1.0;
                    newSubst = NULL;
                } else {
                    f = substance->texture->indexOfRefraction(collision) / closest->texture->indexOfRefraction(collision);
                    newSubst = closest;
                }
            }
            
            Vector3D c = (_r.uv - normal * (_r.uv * normal)) * f;
            Vector3D b = normal * sqrt(_r.uv.squaredmagnitude() - c.squaredmagnitude());
            
            Vector3D uv = c - b;
            
            refraction = cast(Ray3D(collision, uv), _bounces + 1, newSubst, bounced);
        }
        
        // Debug
        return refraction;
    }
    
    // Send out light rays to each light.
    // Using Lambertian reflectance for now.
    Color localColor = closest->texture->localColor(collision);
    double brightness = 0;
    
    if (reflectivity <= 1 - tolerance) {
        for (int i = 0; i < lights.size(); i++) {
            Vector3D light = lights[i].center - collision;
            
            if (debug == mode::lightz) {
                unsigned char b = 126-126 * light.unitize().z();
                
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
                
                double t;
                if (_pblocker->collides(Ray3D(collision, light), &contacts, &t)) {
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
    
    Color lambertian = brightness * localColor;
    
    return reflection * reflectivity + lambertian * (1-reflectivity);
}