//
//  main.h
//  Charles
//
//  Created by Ben Stolovitz on 12/5/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

// refs:
// - http://en.sfml-dev.org/forums/index.php?topic=3543.0 (uint8 and image class)

#ifndef Charles_main_h
#define Charles_main_h

#include <iostream>
#include <string>
#include <thread>
#include <math.h>
#include <ctime>
#include <SFML/graphics.hpp>
#include <vector>
#include "Color.h"
#include "Point2D.h"
#include "Ray3D.h"
#include "Vector3D.h"
#include "Sphere.h"
#include "RectPrism.h"
#include "SolidTexture.h"
#include "Light.h"

/**
 * Enums
 **/

// TODO set to flags
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
int w = 1024;
int h = 1024;
int totalPixels = w * h;
unsigned char* renderImage = new unsigned char[totalPixels * 4];

int maxBounces = 1;

// Renderer statistics
time_t start;
time_t end;
int collided = 0;
int checks = 0;

state currentState = state::rendering;
int totalRenderedPoints = 0;
bool* renderedPoints = new bool[totalPixels];

// Camera configuration
Ray3D eye(Point3D(-50, 200, 0), Vector3D(0, -0.7, 1).unitize());
double pixelsPerMeter = 10;
double fov = 85;

// Objectspace!
std::vector<PhysicalObject*> objects {};
std::vector<Light> lights {};

/**
 * Function time!
 **/
void render();
Point2D getNextPoint();

Color cast(const Ray3D &r, int _bounces = 0);

#endif
