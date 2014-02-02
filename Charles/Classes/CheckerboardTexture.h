//
//  CheckerboardTexture.h
//  Charles
//
//  Created by Ben Stolovitz on 1/24/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__CheckerboardTexture__
#define __Charles__CheckerboardTexture__

#include <iostream>
#include <cmath>
#include "Texture.h"
#include "Color.h"

class CheckerboardTexture : public Texture {
public:
    // Constructors
    CheckerboardTexture(Color color1 = Color(150), Color color2 = Color(50), double reflectivity = 0, double refractivity = 0, double indexOfRefraction = 1.0);
    
    // Local color
    Color localColor(const Point3D &p);
    
    // Specular Reflection
    bool isReflective(const Point3D &p);
    double reflectivity(const Point3D &p);
    
    // Refraction & transparency
    bool isRefractive(const Point3D &p);
    double refractivity(const Point3D &p);
    double indexOfRefraction(const Point3D &p);
    
protected:
    Color _localColor1 = Color(150);
    Color _localColor2 = Color(50);
    double _reflectivity;
    double _refractivity;
    double _indexOfRefraction;
    
    const double tolerance = 0.00000001;
};


#endif /* defined(__Charles__CheckerboardTexture__) */
