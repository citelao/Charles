//
//  Ray3D.h
//  Charles
//
//  Created by Ben Stolovitz on 12/5/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Ray3D__
#define __Charles__Ray3D__

#include <iostream>
#include "Point3D.h"
#include "Vector3D.h"

class Ray3D {
public:
    Point3D p;
    Vector3D uv;
    
    Ray3D(Point3D p, Vector3D uv)
        :p(p), uv(uv) {};
    
    Ray3D(double x, double y, double z, double dx, double dy, double dz)
        :p(x, y, z), uv(dx, dy, dz) {};
};

#endif /* defined(__Charles__Ray3D__) */
