//
//  Vector3D.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/23/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include <math.h>
#include "Vector3D.h"

Vector3D Vector3D::operator+(const Vector3D &nv) const
{
    return Vector3D(x + nv.x, y + nv.y, z + nv.z);
}

Vector3D Vector3D::operator-(const Vector3D &nv) const
{
    return Vector3D(x - nv.x, y - nv.y, z - nv.z);
}

Vector3D Vector3D::operator/(const double &nv) const
{
    return Vector3D(x / nv, y / nv, z / nv);
}

Vector3D Vector3D::operator*(const double &nv) const
{
    return Vector3D(x * nv, y * nv, z * nv);
}

// Dot product.
double Vector3D::operator*(const Vector3D &nv) const
{
    return x * nv.x + y * nv.y + z * nv.z;
}

double Vector3D::magnitude(void) const
{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3D Vector3D::unit(void) const
{
    return *this / this->magnitude(); // todo idfk
}