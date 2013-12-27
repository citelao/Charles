//
//  Sphere.h
//  Charles
//
//  Created by Ben Stolovitz on 10/31/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Sphere__
#define __Charles__Sphere__

#include <iostream>
#include <math.h>
#include "Point3D.h"
#include "PhysicalObject.h"

class Sphere : public PhysicalObject {
public:
    Point3D center;
    double radius;
    
    bool collides(const Ray3D &r, Point3D* p);
    Vector3D normal(const Point3D &p);
    
    Sphere(const Point3D &_c, const double &_r): center(_c), radius(_r) {};
    Sphere(const double &_x, const double &_y, const double &_z, const double &_r): center(_x, _y, _z), radius(_r) {};
};

#endif /* defined(__Charles__Sphere__) */
