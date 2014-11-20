//
//  CheckerboardTexture.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/24/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "CheckerboardTexture.h"

// Constructors
CheckerboardTexture::CheckerboardTexture(Color color1, Color color2, double reflectivity, double refractivity, double indexOfRefraction) {
    _localColor1 = Color(color1);
    _localColor2 = Color(color2);
    _reflectivity = reflectivity;
    _refractivity = refractivity;
    _indexOfRefraction = indexOfRefraction;
}

// Local color
Color CheckerboardTexture::localColor(const Point3D &p) {
    int squareX = abs((int)round(p.x())) / 25;
    int squareY = abs((int)round(p.y())) / 25;
    int squareZ = abs((int)round(p.z())) / 25;
    
    bool negX = p.x() < 0;
    // TODO
//    bool negY = p.y() < 0;
//    bool negZ = p.z() < 0;
    
    bool blackSquare = (negX) ? ((squareX + squareY + squareZ) % 2 == 0) : !((squareX + squareY + squareZ) % 2 == 0);
    
    return blackSquare ? _localColor1 : _localColor2;
}

// Specular Reflection
bool CheckerboardTexture::isReflective(const Point3D &p) {
    return (_reflectivity > 0 - tolerance);
}

double CheckerboardTexture::reflectivity(const Point3D &p) {
    return _reflectivity;
}

// Refraction & transparency
bool CheckerboardTexture::isRefractive(const Point3D &p) {
    return (_refractivity > 0 - tolerance);
}

double CheckerboardTexture::refractivity(const Point3D &p) {
    return _refractivity;
}

double CheckerboardTexture::indexOfRefraction(const Point3D &p) {
    return _indexOfRefraction;
}