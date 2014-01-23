//
//  Point2D.h
//  Charles
//
//  Created by Ben Stolovitz on 1/23/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Point2D__
#define __Charles__Point2D__

#include <iostream>

class Point2D {
public:
    int x;
    int y;
    
    Point2D(int x, int y)
    : x(x), y(y) {};
    Point2D(double x, double y)
    : x((int)x), y((int)y) {};
};

#endif /* defined(__Charles__Point2D__) */
