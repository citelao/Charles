//
//  SolidTexture.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/23/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "SolidTexture.h"

// Constructors
SolidTexture::SolidTexture(Color color, double reflectivity, double refractivity, double indexOfRefraction) {
    _localColor = Color(color);
    _reflectivity = reflectivity;
    _refractivity = refractivity;
    _indexOfRefraction = indexOfRefraction;
}

// Local color
Color SolidTexture::localColor(const Point3D &p) {
    return _localColor;
}

// Specular Reflection
bool SolidTexture::isReflective(const Point3D &p) {
    return (_reflectivity > 0 - tolerance);
}

double SolidTexture::reflectivity(const Point3D &p) {
    return _reflectivity;
}

// Refraction & transparency
bool SolidTexture::isRefractive(const Point3D &p) {
    return (_refractivity > 0 - tolerance);
}

double SolidTexture::refractivity(const Point3D &p) {
    return _refractivity;
}

double SolidTexture::indexOfRefraction(const Point3D &p) {
    return _indexOfRefraction;
}