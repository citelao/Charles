//
//  Vector3D.h
//  Charles
//
//  Created by Ben Stolovitz on 10/23/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Vector3D__
#define __Charles__Vector3D__

#include <iostream>

class Vector3D {
public:
    // Constructors
    template <class T>
    Vector3D(T x = 0, T y = 0, T z = 0)
    :_x(x), _y(y), _z(z) {};
    
    Vector3D()
    :Vector3D(0, 0, 0) {};
    
    // TODO implement a null vector
    
    // Arithemetic
    Vector3D operator+(const Vector3D &nv) const;
    Vector3D operator-(const Vector3D &nv) const;
    
    Vector3D operator/(const double &nv) const;
    Vector3D operator*(const double &nv) const;
    
    // Dot and cross products, respectively.
    double operator*(const Vector3D &nv) const;
    Vector3D cross(const Vector3D &nv) const;
    
    double squaredmagnitude(void) const;
    double magnitude(void) const;
    Vector3D unitize(void) const;
    
    Vector3D reflect(const Vector3D &nv) const;
    
    const double x() const { return _x; };
    const double y() const { return _y; };
    const double z() const { return _z; };
    
    template <class T>
    const T x() const { return _x; };
    template <class T>
    const T y() const { return _y; };
    template <class T>
    const T z() const { return _z; };
    
private:
    double _x;
    double _y;
    double _z;
};

// Vectors and Points are interchangeable mathwise, but I like keeping variables with different names.
typedef Vector3D Point3D;

#endif /* defined(__Charles__Vector3D__) */
