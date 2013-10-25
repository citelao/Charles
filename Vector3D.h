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
    Vector3D(double _x, double _y, double _z);
    
    // Arithemetic
    Vector3D &operator+(const Vector3D &nv) const;
    Vector3D &operator-(const Vector3D &nv) const;
    
    // Dot and cross products, respectively.
    Vector3D &operator*(const Vector3D &nv) const;
    Vector3D &operator/(const Vector3D &nv) const;
};

#endif /* defined(__Charles__Vector3D__) */
