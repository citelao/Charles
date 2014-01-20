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
        double x;
        double y;
        double z;
    
    // Constructors
    Vector3D(double x = 0, double y = 0, double z = 0)
    :x(x), y(y), z(z) {};
    
    // TODO implement a null vector
    
    // Arithemetic
    Vector3D operator+(const Vector3D &nv) const;
    Vector3D operator-(const Vector3D &nv) const;
    
    Vector3D operator/(const double &nv) const;
    Vector3D operator*(const double &nv) const;
    
    // Dot and cross products, respectively.
    double operator*(const Vector3D &nv) const;
    Vector3D cross(const Vector3D &nv) const;
    
    double magnitude(void) const;
    Vector3D unitize(void) const;
};

// Vectors and Points are interchangeable mathwise, but I like keeping variables with different names.
typedef Vector3D Point3D;

#endif /* defined(__Charles__Vector3D__) */
