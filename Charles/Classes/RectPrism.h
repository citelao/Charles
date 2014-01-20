//
//  RectPrism.h
//  Charles
//
//  Created by Ben Stolovitz on 1/18/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__RectPrism__
#define __Charles__RectPrism__

#include <iostream>
#include "Math.h"
#include "PhysicalObject.h"

class RectPrism: public PhysicalObject {
public:
    bool collides(const Ray3D &r, Point3D* p);
    Vector3D normal(const Point3D &p);
    
    RectPrism(double x, double y, double z, double width, double height, double depth);
    RectPrism(Point3D corner, Vector3D dimensions);
    
private:
    double tolerance = 0.00000001;
    
    Vector3D _dimensions;
    Point3D _mincorner;
    Point3D _maxcorner;
    
    void init(Point3D corner, Vector3D dimensions);
    
};

#endif /* defined(__Charles__RectPrism__) */
