#ifndef RAY_H
#define RAY_H

#include "math\point.h"
#include "math\vector.h"

class Ray
{
public:
	Ray(){}
	Ray(Point o, Vector d);
	~Ray(){}

	Point origin;
	Vector direction;
};

#endif 