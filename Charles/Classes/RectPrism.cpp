//
//  RectPrism.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/18/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "RectPrism.h"

RectPrism::RectPrism(double x, double y, double z, double width, double height, double depth, double reflectivity) {
    init(Point3D(x, y, z), Vector3D(width, height, depth), reflectivity);
}

RectPrism::RectPrism(Point3D corner, Vector3D dimensions, double reflectivity) {
    init(corner, dimensions, reflectivity);
}

void RectPrism::init(Point3D corner, Vector3D dimensions, double reflectivity) {
    if (dimensions.x <= 0 ||
        dimensions.y <= 0 ||
        dimensions.z <= 0) {
        throw std::invalid_argument("Rectangular prisms cannot have negative dimensions.");
    }
    
    _reflectivity = reflectivity;
    
    _mincorner = corner;
    _dimensions = dimensions;
    
    _maxcorner = _mincorner + _dimensions;
    
}

bool RectPrism::collides(const Ray3D &r, Point3D* p) {
    double t;
    double lowT = INFINITY;
    Point3D i;
    Point3D lowI;
    
    // Check for front face
    t = (_mincorner.z - r.p.z) / r.uv.z;
    i = r.traverse(t).p;
    
    if (_mincorner.x <= i.x && i.x <= _maxcorner.x &&
        _mincorner.y <= i.y && i.y <= _maxcorner.y &&
        t < lowT && t > 0) {
        lowI = i;
        lowT = t;
    }
    
    // Check for back face
    t = (_maxcorner.z - r.p.z) / r.uv.z;
    i = r.traverse(t).p;
    
    if (_mincorner.x <= i.x && i.x <= _maxcorner.x &&
        _mincorner.y <= i.y && i.y <= _maxcorner.y &&
        t < lowT && t > 0) {
        lowI = i;
        lowT = t;
    }
    
    // Check for left face
    t = (_mincorner.y - r.p.y) / r.uv.y;
    i = r.traverse(t).p;
    
    if (_mincorner.x <= i.x && i.x <= _maxcorner.x &&
        _mincorner.z <= i.z && i.z <= _maxcorner.z &&
        t < lowT && t > 0) {
        lowI = i;
        lowT = t;
    }
    
    // Check for right face
    t = (_maxcorner.y - r.p.y) / r.uv.y;
    i = r.traverse(t).p;
    
    if (_mincorner.x <= i.x && i.x <= _maxcorner.x &&
        _mincorner.z <= i.z && i.z <= _maxcorner.z &&
        t < lowT && t > 0) {
        lowI = i;
        lowT = t;
    }
    
    // Check for top face
    t = (_maxcorner.x - r.p.x) / r.uv.x;
    i = r.traverse(t).p;
    
    if (_mincorner.y <= i.y && i.y <= _maxcorner.y &&
        _mincorner.z <= i.z && i.z <= _maxcorner.z &&
        t < lowT && t > 0) {
        lowI = i;
        lowT = t;
    }
    
    // Check for bottom face
    t = (_mincorner.x - r.p.x) / r.uv.x;
    i = r.traverse(t).p;
    
    if (_mincorner.y <= i.y && i.y <= _maxcorner.y &&
        _mincorner.z <= i.z && i.z <= _maxcorner.z &&
        t < lowT && t > 0) {
        lowI = i;
        lowT = t;
    }
    
    if (lowT != INFINITY) {
        *p = lowI;
        return true;
    }
    
    return false;
}

Vector3D RectPrism::normal(const Point3D &p) {
    // On front face?
    if (fabs(p.z - _mincorner.z) < tolerance) {
        return Vector3D(0, 0, -1);
    }
    
    // On back face?
    if (fabs(p.z - _maxcorner.z) < tolerance) {
        return Vector3D(0, 0, 1);
    }
    
    // On left face?
    if (fabs(p.y -  _mincorner.y) < tolerance) {
        return Vector3D(0, -1, 0);
    }
    
    // On right face?
    if (fabs(p.y - _maxcorner.y) < tolerance) {
        return Vector3D(0, 1, 0);
    }
    
    // On top face?
    if (fabs(p.x - _maxcorner.x) < tolerance) {
        return Vector3D(1, 0, 0);
    }
    
    // On bottom face?
    if (fabs(p.x - _mincorner.x) < tolerance) {
        return Vector3D(-1, 0, 0);
    }
    
    return Vector3D(0, 0, 1);
}

double RectPrism::reflectivity(const Point3D &p) {
    return _reflectivity;
}

Color RectPrism::color(const Point3D &p) {
    return _color;
}