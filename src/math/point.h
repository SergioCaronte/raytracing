#ifndef MATH_POINT_H
#define MATH_POINT_H

#include <iostream>
#include "vector.h"


class Point 
{

public:
    float x, y, z, w;

    Point(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : x(x), y(y), z(z), w(w)
	{}
	
    static double distance(const Point &p1, const Point &p2) 
	{
        return std::sqrt(std::pow(p1.x - p2.x, 2)
                + std::pow(p1.y - p2.y, 2)
                + std::pow(p1.z - p2.z, 2));
    }

    Point &operator+=(const Vector &rhv) 
	{
        this->x += rhv.x;
        this->y += rhv.y;
        this->z += rhv.z;
        this->w += rhv.w;
        return *this;
    }

    Point &operator-=(const Vector &rhv) 
	{
        this->x -= rhv.x;
        this->y -= rhv.y;
        this->z -= rhv.z;
        this->w -= rhv.w;
        return *this;
    }
};


/// + operator for point-vector addition.
inline Point operator+(Point lhv, const Vector &rhv) 
{
    lhv += rhv;
    return lhv;
}

inline Point operator-(Point lhv, const Vector &rhv) 
{
    lhv -= rhv;
    return lhv;
}

inline Point operator*(Point lhv, const float &rhv) 
{
    lhv.x *= rhv;
    lhv.y *= rhv;
    lhv.z *= rhv;
    return lhv;
}

inline Vector operator-(const Point &lhv, const Point &rhv)
{
    return Vector(lhv.x - rhv.x, lhv.y - rhv.y, lhv.z - rhv.z, lhv.w - rhv.w);
}

inline bool operator==(const Point &lhv, const Point &rhv) 
{
    return(lhv.x == rhv.x && lhv.y == rhv.y && lhv.z == rhv.z && lhv.w == rhv.w);
}

inline bool operator!=(const Point &lhv, const Point &rhv) 
{
    return !operator==(lhv, rhv);
}


inline std::ostream &operator<<(std::ostream &stream, const Point &pt) 
{
    stream << "(" << pt.x << ", " << pt.y << ", " << pt.z << ", " << pt.w << ")";
    return stream;
}

#endif
