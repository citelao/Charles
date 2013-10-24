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
#include <SFML/graphics.hpp>

sf::Uint8 * render(int w, int h);
sf::Uint8 *cast(int x, int y);

int w = 640;
int h = 480;

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
            sf::Uint8 *ray = cast(x, y);
            
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

sf::Uint8 *cast(int x, int y)
{
    sf::Uint8 *ray = new sf::Uint8[4];
    ray[0] = std::rand()*255;
    ray[1] = std::rand()*255;
    ray[2] = std::rand()*255;
    ray[3] = 255;
    
    return ray;
}

// cast
    // if collide:
        // bounce towards light source
    // if light source:
        // light
    // if no collide:
        // bg