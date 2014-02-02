//
//  Camera.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/24/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "Camera.h"

Camera::Camera(Ray3D eye, Vector2D screen, double fov, double pixelsPerMeter)
: eye(eye), screen(screen), fov(fov), pixelsPerMeter(pixelsPerMeter){
    // width / (2 * tan(fov / 2))
    screenDistance = screen.x / (2 * tan((M_PI * fov / 180) / 2));
}


Ray3D Camera::translate(Point2D p2d) {
    Vector3D offset = Vector3D(p2d.x / pixelsPerMeter, p2d.y / pixelsPerMeter, 0);

    // Create a coordinate system relative to eye ray.
    // Z' is eye's uv.
    // X' is parallel to X-Z plane, at least for now. Arbitrary rotation to come later. (TODO)
    // X' is also perpendicular to Z', the eye's vector. Their dot product is 0.
    // Y' should be Z' cross X'.
    // Thanks Morgan and Thomas Redding!
    Vector3D cZPrime = eye.uv;
    Vector3D cXPrime = Vector3D(cZPrime.z, 0, -cZPrime.x);
    Vector3D cYPrime = cZPrime.cross(cXPrime);

    Vector3D p = (cXPrime * offset.x) +
    (cYPrime * offset.y) +
    (cZPrime * offset.z);

    Vector3D uv = ((eye.traverse(screenDistance).p + p) - eye.p).unitize();

    // Start from eye
    return Ray3D(eye.p, uv);
}

Point2D Camera::translate(Point3D p) {
//    Ray3D to = Ray3D(eye.p, (p - eye.p).unitize()).traverse(screenDistance);
//
//    Vector3D orth = to.p - eye.traverse(screenDistance).p;
    
    Ray3D n = eye.traverse(screenDistance);
    Vector3D pr = p - n.p;
    Vector3D proj = eye.uv * (eye.uv * pr) / eye.uv.magnitude();
    Vector3D orth = pr - proj;
    
    Vector3D screen = orth.unitize() * (n.p.magnitude() * orth.magnitude() / (n.p.magnitude() + proj.magnitude()));
    
    return Point2D(screen.x * pixelsPerMeter, screen.y * pixelsPerMeter);
}