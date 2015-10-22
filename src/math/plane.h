#ifndef MATH_PLANE_H
#define MATH_PLANE_H

#include "math.h"
#include "Point.h"
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
