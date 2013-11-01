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
#include "Point3D.h"

class Sphere {
public:
    Point3D center;
    double radius;
    
    Sphere(Point3D _c = 0, double _r = 0): center(_c), radius(_r) {};
    Sphere(double _x, double _y, double _z, double _r = 0): center(_x, _y, _z), radius(_r) {};
};


#endif /* defined(__Charles__Sphere__) */
