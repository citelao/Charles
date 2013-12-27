//
//  RectPrism.h
//  Charles
//
//  Created by Ben Stolovitz on 12/12/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__RectPrism__
#define __Charles__RectPrism__

#include <iostream>
#include "PhysicalObject.h"

class RectPrism : public PhysicalObject
{
public:
    // TODO reimplement setters so xyz get updated
    Point3D corner;
    Vector3D dimensions;
    
    bool collides(const Ray3D &r, Point3D* p);
    Vector3D normal(const Point3D &p);
    
    RectPrism(const Point3D &_c, const Vector3D &_d);
    RectPrism(const double &_x, const double &_y, const double &_z, const double &_w, const double &_h, const double &_l)
    : RectPrism(Point3D(_x, _y, _z), Vector3D(_w, _h, _l)) {};
    
protected:
    double x[2];
    double y[2];
    double z[2];
};

#endif /* defined(__Charles__RectPrism__) */
