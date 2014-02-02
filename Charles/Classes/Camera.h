//
//  Camera.h
//  Charles
//
//  Created by Ben Stolovitz on 1/24/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Camera__
#define __Charles__Camera__

#include <iostream>
#include <math.h>
#include "Ray3D.h"
#include "Vector2D.h"

class Camera {
public:
    Camera(Ray3D eye, Vector2D screen, double fov = 85, double pixelsPerMeter = 1);
    
    Ray3D eye;
    Vector2D screen;
    double fov = 85;
    double pixelsPerMeter = 1;
    
    Ray3D translate(Point2D p);
    Point2D translate(Point3D p);
    
//protected:
    double screenDistance;
};

#endif /* defined(__Charles__Camera__) */
