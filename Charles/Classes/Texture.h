//
//  Texture.h
//  Charles
//
//  Created by Ben Stolovitz on 1/23/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Texture__
#define __Charles__Texture__

#include <iostream>
#include "Color.h"
#include "Vector3D.h"

class Texture {
public:
    // Local color
    virtual Color localColor(const Point3D &p) = 0;
    
    // Specular Reflection
    virtual bool isReflective(const Point3D &p) = 0;
    virtual double reflectivity(const Point3D &p) = 0;
    
    // Refraction & transparency
    virtual bool isRefractive(const Point3D &p) = 0;
    virtual double refractivity(const Point3D &p) = 0;
    
    // Emission?
    // ???
};

#endif /* defined(__Charles__Texture__) */
