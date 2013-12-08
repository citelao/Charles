//
//  Sphere.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/31/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Sphere.h"

bool Sphere::collides(const Ray3D &r, Point3D* p) {
    // Use the quadratic equation to solve for sphere intersection.
    double a = pow(r.uv.x, 2) + pow(r.uv.y, 2) + pow(r.uv.z, 2);
    double b = 2 * ((r.p.x - center.x) * r.uv.x + (r.p.y - center.y) * r.uv.y + (r.p.z - center.z) * r.uv.z);
    double c = pow(r.p.x - center.x, 2) + pow(r.p.y - center.y, 2) + pow(r.p.z - center.z, 2) - pow(radius, 2);
    
    // double far = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double near = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    
    if(isnan(near)) {
        return false;
    }
    
    if(near < 0) {
        return false;
    }
    
    Point3D cp = r.p + r.uv * near;
    
//    if((r.p - cp).magnitude()  0) {
//        std::cout<< "test";
//    }
    
    *p = cp;
    
    return true;
}

Vector3D Sphere::normal(const Point3D &p) {
    return p - center;
}
