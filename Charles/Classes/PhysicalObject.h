//
//  PhysicalObject.h
//  Charles
//
//  Created by Ben Stolovitz on 12/5/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#ifndef __Charles__PhysicalObject__
#define __Charles__PhysicalObject__

#include <iostream>
#include "Point3D.h"
#include "Vector3D.h"
#include "Ray3D.h"

class PhysicalObject {
public:
    virtual bool collides(const Ray3D &r, Point3D* p);
    virtual Vector3D normal(const Point3D &p);
    virtual ~PhysicalObject() {}
};

#endif /* defined(__Charles__PhysicalObject__) */