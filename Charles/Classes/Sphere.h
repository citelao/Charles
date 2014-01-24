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
#include "PhysicalObject.h"

class Sphere : public PhysicalObject {
public:
    bool collides(const Ray3D &r, Point3D* p);
    Vector3D normal(const Point3D &p);
    
    Sphere(Texture *texture, Point3D _c = 0, double _r = 0): PhysicalObject(texture), center(_c), radius(_r) {};
    Sphere(Texture *texture, double _x, double _y, double _z, double _r = 0): PhysicalObject(texture), center(_x, _y, _z), radius(_r) {};
    
protected:
    Point3D center;
    double radius;
    double _reflectivity;
    Color _color = Color(0, 130, 255);
};

#endif /* defined(__Charles__Sphere__) */
