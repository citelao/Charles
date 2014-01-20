//
//  Color.h
//  Charles
//
//  Created by Ben Stolovitz on 1/19/14.
//  Copyright (c) 2014 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__Color__
#define __Charles__Color__

#include <iostream>


class Color {
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a = 255;
    
    Color(unsigned char _r, unsigned char _g, unsigned char _b)
    : r(_r), g(_g), b(_b) {};
    
    Color(int _r, int _g, int _b);
    Color(double _r, double _g, double _b);
    
    // TODO global colors (white, black, etc)
};

#endif /* defined(__Charles__Color__) */
