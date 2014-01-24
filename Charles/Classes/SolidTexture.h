//
//  SolidTexture.h
//  Charles
//
//  Created by Ben Stolovitz on 1/23/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__SolidTexture__
#define __Charles__SolidTexture__

#include <iostream>
#include "Texture.h"
#include "Color.h"

class SolidTexture : public Texture {
public:
    // Constructors
    SolidTexture(Color color = Color(255, 255, 255), double reflectivity = 0, double refractivity = 0);
    
    // Local color
    Color localColor(const Point3D &p);
    
    // Specular Reflection
    bool isReflective(const Point3D &p);
    double reflectivity(const Point3D &p);
    
    // Refraction & transparency
    bool isRefractive(const Point3D &p);
    double refractivity(const Point3D &p);
    
protected:
    Color _localColor = Color(0);
    double _reflectivity;
    double _refractivity;
    
    const double tolerance = 0.00000001;
};

#endif /* defined(__Charles__SolidTexture__) */
