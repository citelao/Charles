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
#include <math.h>
#include <SFML/graphics.hpp>

sf::Uint8 * render(int w, int h);
sf::Uint8 *cast(int x, int y, int b);

int w = 400;
int h = 400;

int main(int argc, const char * argv[])
{
    std::cout << "Go, Charles!\n";
    
    // Create window.
    sf::RenderWindow window(sf::VideoMode(w, h), "Charles");
    
    
    // Instantiate texture and sprite.
    sf::Texture renderTexture;
    sf::Sprite renderSprite;
    
    renderTexture.create(w, h);
    
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
        
        // RAYTRACE!
        // TODO threaded async http://en.cppreference.com/w/cpp/thread
        sf::Uint8 *rawRender = render(w, h);
        
        // Write frame.
        renderTexture.update(rawRender);
        renderSprite.setTexture(renderTexture);
        
        // Cleanup.
        delete [] rawRender;
        
        // Display the frame
        window.draw(renderSprite);
        window.display();
    }
    
    return 0;
}

sf::Uint8 *render(int w, int h)
{
    sf::Uint8 *raw = new sf::Uint8[w * h * 4];
    
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            // Cast ray.
            sf::Uint8 *ray = cast(x, y, 0);
            
            // Store color of pixel.
            raw[(x * h + y) * 4] = ray[0];
            raw[(x * h + y) * 4 + 1] = ray[1];
            raw[(x * h + y) * 4 + 2] = ray[2];
            raw[(x * h + y) * 4 + 3] = ray[3];
            
            delete [] ray;
        }
    }
    
    return raw;
}

sf::Uint8 *cast(int x, int y, int b)
{
    sf::Uint8 *ray = new sf::Uint8[4];
    
    if (b >= 6) {
        ray[2] = 200;
    } else if (pow((double)(x-200),2) + pow((double)(y-200),2) <= pow(50, 2)) { // Hit circle.
        // Recast ray
        sf::Uint8 *rnew = cast(((x-200)*4)+200, y, b+1);
        ray[2] = rnew[2] + 50;
        delete [] rnew;
    } else if(x > 300) {
    // Hit floor.
        ray[2] = 150;
    } else {
    // Hit ceiling.
        ray[2] = 50;
    }
    
    // if collide:
    // bounce towards light source
    // if light source:
    // light
    // if no collide:
    // bg
    
    ray[0] = 0;
    ray[1] = 0;
    ray[3] = 255;
    
    return ray;
}