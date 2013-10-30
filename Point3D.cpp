//
//  Point3D.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/29/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Point3D.h"

Vector3D & Point3D::operator-(const Point3D &np) const {
    return *new Vector3D(this->x - np.x, this->y - np.y, this->z - np.z);
}