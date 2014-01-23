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

Color::Color(int _c) {
    r = (unsigned char)_c;
    g = (unsigned char)_c;
    b = (unsigned char)_c;
}

Color::Color(double _c) {
    r = (unsigned char)_c;
    g = (unsigned char)_c;
    b = (unsigned char)_c;
}

Color Color::operator+(const Color &c) const {
    double _r = r + c.r;
    double _g = g + c.g;
    double _b = b + c.b;
    
    if (_r > 255) {
        _r = 255;
    }
    
    if (_g > 255) {
        _g = 255;
    }
    
    if (_b > 255) {
        _b = 255;
    }
    
    return Color(_r, _g, _b);
}

Color Color::operator+(const int &c) const {
    return Color(c, c, c) + *this;
}

Color Color::operator+(const double &c) const {
    return Color(c, c, c) + *this;
}

Color Color::operator*(const Color &c) const {
    double _dr = (double)r * (double)c.r / 255;
    double _dg = (double)g * (double)c.g / 255;
    double _db = (double)b * (double)c.b / 255;
    
    return Color(_dr, _dg, _db);
}

Color Color::operator*(const int &c) const {
    return Color(c) * *this;
}

Color Color::operator*(const double &c) const {
    int _color = 255 * c;
    
    return Color(_color) * *this;
}