//
//  Vector2D.h
//  Charles
//
//  Created by Ben Stolovitz on 1/23/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Vector2D__
#define __Charles__Vector2D__

#include <iostream>

class Vector2D {
public:
    int x;
    int y;
    
    Vector2D(int x, int y)
    : x(x), y(y) {};
    Vector2D(double x, double y)
    : x((int)x), y((int)y) {};
};
using Point2D = Vector2D;

#endif /* defined(__Charles__Point2D__) */
