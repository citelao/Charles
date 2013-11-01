//
//  Light.h
//  Charles
//
//  Created by Ben Stolovitz on 11/1/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Light__
#define __Charles__Light__

#include <iostream>
#include "Point3D.h"

class Light {
public:
    Point3D center;
    double intensity;
    
    Light(Point3D _c = 0, double _r = 0): center(_c), intensity(_r) {};
    Light(double _x, double _y, double _z, double _r = 0): center(_x, _y, _z), intensity(_r) {};
};

#endif /* defined(__Charles__Light__) */
