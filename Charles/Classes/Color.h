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
    // TODO global colors (white, black, etc)
    
    template <class T>
    Color(T r, T g, T b) {
        _r = (unsigned char)r;
        _g = (unsigned char)g;
        _b = (unsigned char)b;
        _a = 255;
    };
    
    template <class T>
    Color(T c) {
        _r = (unsigned char)c;
        _g = (unsigned char)c;
        _b = (unsigned char)c;
        _a = 255;
    };
    
    // Operators
    template <class T, class U>
    friend Color operator+(const T &c1, const U &c2) {
        return Color(c1) + Color(c2);
    };
    friend Color operator+(const Color &c1, const Color &c2);
    
    template <class T, class U>
    friend Color operator*(const T &c1, const U &c2) {
        return Color(c1) * Color(c2);
    };
    friend Color operator*(const Color &c1, const Color &c2);
    
    // Doubles behave on a scale of 0-1, not 0-255, so we have special
    // methods that don't just cast (unsigned char).
    friend Color operator*(const Color &c1, const double &c2);
    friend Color operator*(const double &c1, const Color &c2);
    
    const unsigned char r() { return r<unsigned char>(); };
    const unsigned char g() { return g<unsigned char>(); };
    const unsigned char b() { return b<unsigned char>(); };
    const unsigned char a() { return a<unsigned char>(); };
    
    template <class T>
    const T r() { return (T) _r; };
    
    template <class T>
    const T g() { return (T) _g; };
    
    template <class T>
    const T b() { return (T) _b; };
    
    template <class T>
    const T a() { return (T) _a; };
    
private:
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;
    unsigned char _a;
};

#endif /* defined(__Charles__Color__) */
