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

inline std::ostream &operator<<(std::ostream &stream, const Ray &ray) 
{
	stream << "origin: (" <<ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << ") direction: ("
		<< ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")";
    return stream;
}

#endif 