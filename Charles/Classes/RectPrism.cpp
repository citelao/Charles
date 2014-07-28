//
//  RectPrism.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/18/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "RectPrism.h"

RectPrism::RectPrism(Texture *texture, double x, double y, double z, double width, double height, double depth)
: PhysicalObject(texture){
    init(Point3D(x, y, z), Vector3D(width, height, depth));
}

RectPrism::RectPrism(Texture *texture, Point3D corner, Vector3D dimensions)
: PhysicalObject(texture) {
    init(corner, dimensions);
}

void RectPrism::init(Point3D corner, Vector3D dimensions) {
    if (dimensions.x() <= 0 ||
        dimensions.y() <= 0 ||
        dimensions.z() <= 0) {
        throw std::invalid_argument("Rectangular prisms cannot have negative dimensions.");
    }

    _mincorner = corner;
    _dimensions = dimensions;
    
    _maxcorner = _mincorner + _dimensions;
}

bool RectPrism::collides(const Ray3D &r, Point3D *p, double *t) {
    double _t;
    double lowT = INFINITY;
    Point3D i;
    Point3D lowI;
    
    // Check for front face
    _t = (_mincorner.z() - r.p.z()) / r.uv.z();
    i = r.traverse(_t).p;
    
    if (_mincorner.x() <= i.x() && i.x() <= _maxcorner.x() &&
        _mincorner.y() <= i.y() && i.y() <= _maxcorner.y() &&
        _t < lowT && _t > 0) {
        lowI = i;
        lowT = _t;
    }
    
    // Check for back face
    _t = (_maxcorner.z() - r.p.z()) / r.uv.z();
    i = r.traverse(_t).p;
    
    if (_mincorner.x() <= i.x() && i.x() <= _maxcorner.x() &&
        _mincorner.y() <= i.y() && i.y() <= _maxcorner.y() &&
        _t < lowT && _t > 0) {
        lowI = i;
        lowT = _t;
    }
    
    // Check for left face
    _t = (_mincorner.y() - r.p.y()) / r.uv.y();
    i = r.traverse(_t).p;
    
    if (_mincorner.x() <= i.x() && i.x() <= _maxcorner.x() &&
        _mincorner.z() <= i.z() && i.z() <= _maxcorner.z() &&
        _t < lowT && _t > 0) {
        lowI = i;
        lowT = _t;
    }
    
    // Check for right face
    _t = (_maxcorner.y() - r.p.y()) / r.uv.y();
    i = r.traverse(_t).p;
    
    if (_mincorner.x() <= i.x() && i.x() <= _maxcorner.x() &&
        _mincorner.z() <= i.z() && i.z() <= _maxcorner.z() &&
        _t < lowT && _t > 0) {
        lowI = i;
        lowT = _t;
    }
    
    // Check for top face
    _t = (_maxcorner.x() - r.p.x()) / r.uv.x();
    i = r.traverse(_t).p;
    
    if (_mincorner.y() <= i.y() && i.y() <= _maxcorner.y() &&
        _mincorner.z() <= i.z() && i.z() <= _maxcorner.z() &&
        _t < lowT && _t > 0) {
        lowI = i;
        lowT = _t;
    }
    
    // Check for bottom face
    _t = (_mincorner.x() - r.p.x()) / r.uv.x();
    i = r.traverse(_t).p;
    
    if (_mincorner.y() <= i.y() && i.y() <= _maxcorner.y() &&
        _mincorner.z() <= i.z() && i.z() <= _maxcorner.z() &&
        _t < lowT && _t > 0) {
        lowI = i;
        lowT = _t;
    }
    
    if (lowT != INFINITY) {
        *p = lowI;
        *t = lowT;
        return true;
    }
    
    return false;
}

Vector3D RectPrism::normal(const Point3D &p) {
    // On front face?
    if (fabs(p.z() - _mincorner.z()) < tolerance) {
        return Vector3D(0, 0, -1);
    }
    
    // On back face?
    if (fabs(p.z() - _maxcorner.z()) < tolerance) {
        return Vector3D(0, 0, 1);
    }
    
    // On left face?
    if (fabs(p.y() -  _mincorner.y()) < tolerance) {
        return Vector3D(0, -1, 0);
    }
    
    // On right face?
    if (fabs(p.y() - _maxcorner.y()) < tolerance) {
        return Vector3D(0, 1, 0);
    }
    
    // On top face?
    if (fabs(p.x() - _maxcorner.x()) < tolerance) {
        return Vector3D(1, 0, 0);
    }
    
    // On bottom face?
    if (fabs(p.x() - _mincorner.x()) < tolerance) {
        return Vector3D(-1, 0, 0);
    }
    
    return Vector3D(0, 0, 0);
}