/*
* Copyright (C) 2015 Sergio Nunes da Silva Junior 
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* author: Sergio Nunes da Silva Junior
* contact: sergio.nunes@dcc.ufmg.com.br
* Universidade Federal de Minas Gerais (UFMG) 
*/
#ifndef MATH_PLANE_H
#define MATH_PLANE_H

#include "math.h"
#include "point.h"
#include "vector.h"

class Plane 
{
public:
    float a, b, c, d;

    Plane(float _a = 0.0f, float _b = 0.0f, float _c = 0.0f, float _d = 0.0f)
        : a(_a), b(_b), c(_c), d(_d) 
	{}


    inline Vector normal() const 
	{
        return Vector(a, b, c);
    }
};

#endif
