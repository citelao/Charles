//
//  Camera.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/24/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "Camera.h"

Camera::Camera(Ray3D eye, Vector2D screen, double fov, double pixelsPerMeter)
: eye(eye), screen(screen), fov(fov), pixelsPerMeter(pixelsPerMeter) {
    // construct the appropriate triangle that has the requested FOV.
    // width / (2 * tan(fov / 2))
    screenDistance = screen.x / (2 * tan((M_PI * fov / 180) / 2));
}


Ray3D Camera::translate(Point2D p2d) {
    Vector3D offset = Vector3D(p2d.x / pixelsPerMeter, p2d.y / pixelsPerMeter, 0.0);

    // Create a coordinate system relative to eye ray.
    // Z' is eye's uv.
    // X' is parallel to X-Z plane, at least for now. Arbitrary rotation to come later. (TODO)
    // X' is also perpendicular to Z', the eye's vector. Their dot product is 0.
    // Y' should be Z' cross X'.
    // Thanks Morgan and Thomas Redding!
    Vector3D cZPrime = eye.uv;
    Vector3D cXPrime = Vector3D(cZPrime.z(), 0.0, -cZPrime.x());
    Vector3D cYPrime = cZPrime.cross(cXPrime);

    Vector3D p = (cXPrime * offset.x()) +
    (cYPrime * offset.y()) +
    (cZPrime * offset.z());

    Vector3D uv = ((eye.traverse(screenDistance).p + p) - eye.p).unitize();

    // Start from eye
    // TODO allow start from near plane
    // return Ray3D(eye.p + screenDistance * uv, uv);
    return Ray3D(eye.p, uv);
}

Point2D Camera::translate(Point3D p) {
//    Ray3D to = Ray3D(eye.p, (p - eye.p).unitize()).traverse(screenDistance);
//
//    Vector3D orth = to.p - eye.traverse(screenDistance).p;
    
//    p = Vector3D(-200, 00, 300);
    
    // Extend a vector from the desired point to the eye
    // and dot it with the screen's middle to produce the correct screen offset.
    // create a vector with that magnitude and figure its difference
    // from the screen's middle. That will produce the screen location.
    Vector3D middle = eye.uv * screenDistance;
    Vector3D pr = p - eye.p;
    Vector3D proj = (eye.uv.unitize() * pr) * pr.unitize();
    Vector3D adj = (eye.uv.unitize() * pr) * eye.uv.unitize();
    Vector3D orth = proj - middle;
    
    Vector3D screen = orth;

//    Vector3D pr = p - eye.p;
//    Vector3D cZPrime = eye.uv;
//    Vector3D cXPrime = Vector3D(cZPrime.z(), 0.0, -cZPrime.x());
//    Vector3D cYPrime = cZPrime.cross(cXPrime);
//    Vector3D screen = (cXPrime * pr.x()) +
//    (cYPrime * pr.y()) +
//    (cZPrime * pr.z());
    
//    Vector3D n = eye.p + screenDistance * eye.uv;
//    Vector3D pr = p - eye.p;
//    Vector3D proj = eye.uv.unitize() * (eye.uv * pr) / eye.uv.magnitude(); // this is wrong
//    Vector3D orth = pr - proj;

//    Vector3D screen = orth.unitize() * (n.magnitude() * orth.magnitude() / proj.magnitude());

    return Point2D(screen.x() * pixelsPerMeter, screen.y() * pixelsPerMeter);
}