//
//  Color.cpp
//  Charles
//
//  Created by Ben Stolovitz on 1/19/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#include "Color.h"

Color operator+(const Color &c1, const Color &c2) {
    double r = c1._r + c2._r;
    double g = c1._g + c2._g;
    double b = c1._b + c2._b;
    
    if (r > 255) {
        r = 255;
    }
    
    if (g > 255) {
        g = 255;
    }
    
    if (b > 255) {
        b = 255;
    }
    
    return Color(r, g, b);
}

Color operator*(const Color &c1, const Color &c2) {
    double _dr = (double)c1._r * (double)c2._r / 255;
    double _dg = (double)c1._g * (double)c2._g / 255;
    double _db = (double)c1._b * (double)c2._b / 255;
    
    return Color(_dr, _dg, _db);
}

Color operator*(const Color &c1, const double &c2) {
    return c1 * Color(c2 * 255);
}
Color operator*(const double &c1, const Color &c2) {
    return c2 * c1; // Avoid duplication
}