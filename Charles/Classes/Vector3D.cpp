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
    return Vector3D(_x + nv._x, _y + nv._y, _z + nv._z);
}

Vector3D Vector3D::operator-(const Vector3D &nv) const
{
    return Vector3D(_x - nv._x, _y - nv._y, _z - nv._z);
}

Vector3D Vector3D::operator/(const double &nv) const
{
    return Vector3D(_x / nv, _y / nv, _z / nv);
}

Vector3D Vector3D::operator*(const double &nv) const
{
    return Vector3D(_x * nv, _y * nv, _z * nv);
}

// Dot product.
double Vector3D::operator*(const Vector3D &nv) const
{
    return _x * nv._x + _y * nv._y + _z * nv._z;
}

// Cross product
Vector3D Vector3D::cross(const Vector3D &nv) const
{
    return Vector3D(_y * nv._z - _z * nv._y,
                    _z * nv._x - _x * nv._z,
                    _x * nv._y - _y * nv._x);
}

double Vector3D::squaredmagnitude(void) const
{
    return pow(_x, 2) + pow(_y, 2) + pow(_z, 2);
}

double Vector3D::magnitude(void) const
{
    return sqrt(this->squaredmagnitude());
}

Vector3D Vector3D::unitize(void) const
{
    return *this / this->magnitude(); // todo idfk
}

Vector3D Vector3D::reflect(const Vector3D &normal) const
{
    return *this + normal * 2;
}