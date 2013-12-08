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
#include <SFML/graphics.hpp>
#include <vector>
#include "Ray3D.h"
#include "Vector3D.h"
#include "Point3D.h"
#include "Sphere.h"
#include "Light.h"

/**
 * The ever-useful color struct
 **/
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

/**
 * Define them variables.
 **/
bool debug = false;
int collided = 0;
int checks = 0;
int threads;

int w = 512;
int h = 512;
unsigned char *renderImage = new unsigned char[w * h * 4];

Point3D screenPos(- w / 2, - h / 2, 0); // TODO, since we use projection now.
Point3D camera(0, 0, - 500);

std::vector<PhysicalObject*> objects {};
std::vector<Light> lights {};

/**
 * Function time!
 **/
void render(int _x, int _y, int _w, int _h);
Color cast(const Ray3D &r, int _bounces = 0);

#endif
