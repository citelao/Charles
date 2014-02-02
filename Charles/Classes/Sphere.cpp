//
//  Sphere.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/31/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Sphere.h"

bool Sphere::collides(const Ray3D &r, Point3D *p, double *t) {
    // Use the quadratic equation to solve for sphere intersection.
    double a = r.uv * r.uv;
    double b = 2 * ((r.p - center) * r.uv);
    double c = (r.p - center) * (r.p - center) - pow(radius, 2);
    
//    double far = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double near = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    
    if(isnan(near)) {
        return false;
    }
    
    if(near < 0) {
        return false;
    }
    
    Point3D cp = r.traverse(near).p;
    
    *p = cp;
    *t = near;
    
    return true;
}

Vector3D Sphere::normal(const Point3D &p) {
    return (p - center).unitize();
}