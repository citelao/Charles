//
//  Point3D.h
//  Charles
//
//  Created by Ben Stolovitz on 10/29/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Point3D__
#define __Charles__Point3D__

#include <iostream>
#include "Vector3D.h"

class Point3D {
public:
    double x;
    double y;
    double z;
    
    Point3D(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {};
    
    Vector3D minus(const Point3D &np) const;
    
    Vector3D &operator-(const Point3D &np) const;
};

#endif /* defined(__Charles__Point3D__) */
