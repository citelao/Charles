//
//  Ray3D.cpp
//  Charles
//
//  Created by Ben Stolovitz on 12/5/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Ray3D.h"

Point3D Ray3D::interval(const double &t) const {
    double _x = p.x - uv.x * t;
    double _y = p.y - uv.y * t;
    double _z = p.z - uv.z * t;
    
    return Point3D(_x, _y, _z);
}