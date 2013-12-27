//
//  RectPrism.cpp
//  Charles
//
//  Created by Ben Stolovitz on 12/12/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "RectPrism.h"

RectPrism::RectPrism(const Point3D &_c, const Vector3D &_d) {
    corner = _c;
    dimensions = _d;

    bool xcmax = (_d.x < 0);
    x[0] = (xcmax) ? _c.x + _d.x : _c.x;
    x[1] = (xcmax) ? _c.x : _c.x + _d.x;
    
    bool ycmax = (_d.y < 0);
    y[0] = (ycmax) ? _c.y + _d.y : _c.y;
    y[1] = (ycmax) ? _c.y : _c.y + _d.y;
    
    bool zcmax = (_d.z < 0);
    z[0] = (zcmax) ? _c.z + _d.z : _c.z;
    z[1] = (zcmax) ? _c.z : _c.z + _d.z;
}

bool RectPrism::collides(const Ray3D &r, Point3D* p) {
    double t;
    Point3D i;
    
    // Front face
    // If collision at z = low z is w/in face
    t = (r.p.z - z[1]) / r.uv.z;
    i = r.interval(t);
    
    if (y[0] <= i.y && i.y <= y[1] &&
        x[0] <= i.x && i.x <= x[1]) {
        *p = Point3D(i.x, i.y, i.z);
        return true;
    }
    
    // Back face
    t = (r.p.z - z[0]) / r.uv.z;
    i = r.interval(t);
    
    if (y[0] <= i.y && i.y <= y[1] &&
        x[0] <= i.x && i.x <= x[1]) {
        *p = Point3D(i.x, i.y, i.z);
        return true;
    }
    
    // east face
    t = (r.p.x - x[1]) / r.uv.x;
    i = r.interval(t);
    
    if (y[0] <= i.y && i.y <= y[1] &&
        z[0] <= i.z && i.z <= z[1]) {
        *p = Point3D(i.x, i.y, i.z);
        return true;
    }
    
    // west face
    t = (r.p.x - x[0]) / r.uv.x;
    i = r.interval(t);
    
    if (y[0] <= i.y && i.y <= y[1] &&
        z[0] <= i.z && i.z <= z[1]) {
        *p = Point3D(i.x, i.y, i.z);
        return true;
    }
    
    
    return false;
}

Vector3D RectPrism::normal(const Point3D &p) {
    
    // bottom face
    if (p.x == x[0]) {
        return Vector3D(-1,0,0);
    }
    
    // top face
    if (p.x == x[1]) {
        return Vector3D(1,0,0);
    }
    
    if (p.y == y[0]) {
        return Vector3D(0,-1,0);
    }
    
    if (p.y == y[1]) {
        return Vector3D(0,1,0);
    }
    
    // front face
    if (p.z == z[0]) {
        return Vector3D(0,0,1);
    }
    
    // back face
    if (p.z == z[1]) {
        return Vector3D(0,0,-1);
    }
    
    // no collision
    return Vector3D(1,0,0);
}