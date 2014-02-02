//
//  main.h
//  Charles
//
//  Created by Ben Stolovitz on 12/5/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

// refs:
// - http://en.sfml-dev.org/forums/index.php?topic=3543.0 (image class)
// - http://www.sfml-dev.org/documentation/2.0/classsf_1_1VertexArray.php (vertex array)

#ifndef Charles_main_h
#define Charles_main_h

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <math.h>
#include <ctime>
#include <SFML/graphics.hpp>
#include <bitset>
#include <vector>
#include "Color.h"
#include "Vector2D.h"
#include "Ray3D.h"
#include "Vector3D.h"
#include "Camera.h"
#include "Sphere.h"
#include "RectPrism.h"
#include "SolidTexture.h"
#include "CheckerboardTexture.h"
#include "Light.h"

/**
 * Enums
 **/

// TODO change to flags
enum mode {
    none,
    onscreen,    // should print all onscreen objects as white, regardless of illumination
    shadows,     // should help show shadows
    normal,      // should display normal line; r: x, g: y, b: z
    normalz,     // should display normal z value
    lightz,      // should display light ray z value
    unrendered,  // shows unrendered pixels as purple.
};

enum state {
    rendering,
    notifying,
    done
};

/**
 * Define them variables.
 **/

// Debug mode
mode debug = mode::none;

// Math globals that really should be somewhere else (TODO)
double tolerance = 0.00000001;

// Renderer settings
const int w = 1024;
const int h = 512;
const int totalPixels = w * h;
unsigned char* renderImage = new unsigned char[totalPixels * 4];

int maxBounces = 5;

// Renderer statistics
time_t start;
time_t end;
int collided = 0;
int checks = 0;

state currentState = state::rendering;
int totalRenderedPoints = 0;
std::bitset<totalPixels> renderedPoints;

// Camera configuration
Ray3D eye(Point3D(-100, 0, 0), Vector3D(0, 0, 1).unitize());
Vector2D origin(w / 2, h / 2);
double pixelsPerMeter = 1;
double fov = 85;
Camera camera(eye, Vector2D(w, h), fov, pixelsPerMeter);

// Camera controls
Vector2D dragMouseOrigin(0, 0);
Ray3D dragEye(Point3D(0, 0, 0), Vector3D(0, 0, 0));

// Objectspace!
std::vector<PhysicalObject*> objects {};
std::vector<Light> lights {};
std::vector<Point3D> bounces {};

/**
 * SFML vars, too.
 **/

// Create window
sf::RenderWindow window(sf::VideoMode(w, h), "Charles");

// Instantiate texture and sprite.
sf::Texture texture;
sf::Sprite sprite;

// Make a vertex array for the mouse vertex
sf::VertexArray mouseRay(sf::LinesStrip, maxBounces + 1);

/**
 * Function time!
 **/
void render();

Point2D getNextPoint();

Color cast(const Ray3D &r, int _bounces = 0, PhysicalObject *substance = NULL, std::vector<Point3D> *bounced = NULL);

#endif
