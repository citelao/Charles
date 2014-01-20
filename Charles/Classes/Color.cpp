//
//  Color.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/19/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "Color.h"


Color::Color(int _r, int _g, int _b)
{
    r = (unsigned char)_r;
    g = (unsigned char)_g;
    b = (unsigned char)_b;
}

Color::Color(double _r, double _g, double _b) {
    r = (unsigned char)_r;
    g = (unsigned char)_g;
    b = (unsigned char)_b;
}