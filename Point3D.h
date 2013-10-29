//
//  Point3D.h
//  Charles
//
//  Created by Ben Stolovitz on 10/29/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Point3D__
#define __Charles__Point3D__

#include <iostream>

class Point3D {
public:
    double x;
    double y;
    double z;
    
    Point3D(int x = 0, int y = 0, int z = 0): x(x), y(y), z(z) {};
};

#endif /* defined(__Charles__Point3D__) */
