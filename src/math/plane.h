#ifndef MATH_PLANE_H
#define MATH_PLANE_H

#include "math.h"
#include "Point.h"
#include "vector.h"

/**
 * This class represents a plane equation.
 **/
class Plane {

public:
    /// a coefficient of the plane in ax + by + cz = d.
    float a;

    /// b coefficient of the plane in ax + by + cz = d.
    float b;

    /// c coefficient of the plane in ax + by + cz = d.
    float c;

    /// d coefficient of the plane in ax + by + cz = d.
    float d;
#if 0
    /**
     * Construct a plane given three points in it.
     * @param p0 First point in the plane.
     * @param p1 Second point in the plane.
     * @param p2 Third point in the plane.
     **/
    Plane(const Point &p0, const Point &p1, const Point &p2) {

    }

    /**
     * Constructs a plane given a vector normal to the plane and
     * a point in it.
     * @param normal The vector normal to the plane.
     * @param p The point in the plane.
     **/
    Plane(const Vector *normal, const Point *p) {

    }

    /**
     * Constructs a plane given two vectors.
     * @param v0 The first vector in the plane.
     * @param v1 The second vector in the plane.
     **/
    Plane(const Vector &v0, const Vector &v1) {

    }
#endif
    /**
     * Constructs a plane given its 4 coefficients:
     * ax + by + cz + d = 0
     **/
    Plane(float _a = 0.0f, float _b = 0.0f, float _c = 0.0f, float _d = 0.0f)
        : a(_a), b(_b), c(_c), d(_d) {

    }

    /**
     * Returns the normal vector.
     * It may not be normalized.
     **/
    inline Vector normal() const {
        return Vector(a, b, c);
    }

    /**
     * Solves the equation and returns true if the given point is in the plane
     * and false if not.
     * TODO this is buggy.
     **/
    inline bool solve(const Point &p) {
        if(std::abs(p.x * a + p.y * b + p.z * c + d) <= FLT_EPSILON)
            return true;

        return false;
    }
};

#endif // !MATH_PLANE_HPP
