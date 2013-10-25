//
//  Color.cpp
//  Charles
//
//  Created by Ben Stolovitz on 10/24/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Color.h"

class Color {
    public:
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    
        operator unsigned char();
    
        Color(void);
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};