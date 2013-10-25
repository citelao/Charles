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

void render(int _x, int _y, int _w, int _h);

int w = 400;
int h = 400;
sf::Uint8 *renderImage = new sf::Uint8[w * h * 4];

int main(int argc, const char * argv[])
{
    std::cout << "Go, Charles!\n";
    
    // Create window.
    sf::RenderWindow window(sf::VideoMode(w, h), "Charles");
    
    // Instantiate texture and sprite.
    sf::Texture renderTexture;
    sf::Sprite renderSprite;
    
    renderTexture.create(w, h);
    
    // Start render thread.
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
        
        // Write frame.
        renderTexture.update(renderImage);
        
        renderSprite.setTexture(renderTexture);
        
        // Cleanup.
        
        // Display the frame
        window.draw(renderSprite);
        window.display();
    }
    
    return 0;
}

void render(int _x, int _y, int _w, int _h)
{
    std::cout << "Render thread started! \n";
    
    for (int i = _y; i < _y + _h; i++) {
        for (int j = _x; j < _x + _w; j++ ) {
            renderImage[((i * w) + j) * 4]     = 255;
            renderImage[((i * w) + j) * 4 + 1] = 255;
            renderImage[((i * w) + j) * 4 + 2] = 255;
            renderImage[((i * w) + j) * 4 + 3] = 255;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    
    std::cout << "Render thread completed! \n";
}