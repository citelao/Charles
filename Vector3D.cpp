//
//  Vector3D.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/23/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include <math.h>
#include "Vector3D.h"

Vector3D &Vector3D::operator+(const Vector3D &nv) const {
    return *new Vector3D(this->x + nv.x, this->y + nv.y, this->z + nv.z);
}

Vector3D &Vector3D::operator-(const Vector3D &nv) const {
    return *new Vector3D(this->x - nv.x, this->y - nv.y, this->z - nv.z);
}

Vector3D &Vector3D::operator/(const double &nv) const {
    return *new Vector3D(this->x / nv, this->y / nv, this->z / nv);
}

double Vector3D::magnitude(void) {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}