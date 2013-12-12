//
//  Point3D.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/29/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Point3D.h"

Vector3D Point3D::operator-(const Point3D &np) const {
    return Vector3D(x - np.x, y - np.y, z - np.z);
}

Point3D Point3D::operator+(const Vector3D &v) const {
    return Point3D(x + v.x, y + v.y, z + v.z);
}
