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
    double reflectivity(const Point3D &p);
    Color color(const Point3D &p);
    
    RectPrism(double x, double y, double z, double width, double height, double depth, double reflectivity = 0);
    RectPrism(Point3D corner, Vector3D dimensions, double reflectivity = 0);
    
protected:
    double tolerance = 0.00000001;
    
    double _reflectivity;
    Color _color = Color(50, 0, 180);
    
    Vector3D _dimensions;
    Point3D _mincorner;
    Point3D _maxcorner;
    
    void init(Point3D corner, Vector3D dimensions, double reflectivity = 0);
    
};

#endif /* defined(__Charles__RectPrism__) */
