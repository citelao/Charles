//
//  Ray3D.cpp
//  Charles
//
//  Created by Ben Stolovitz on 12/5/13.
//  Copyright (c) 2013 Ben Stolovitz. All rights reserved.
//

#include "Ray3D.h"

Ray3D Ray3D::traverse(const double  amount) const
{
    return Ray3D(
                 Point3D(p.x + uv.x * amount,
                         p.y + uv.y * amount,
                         p.z + uv.z * amount),
                 uv);
}