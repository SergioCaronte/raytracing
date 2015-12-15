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
#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include <iostream>
#include "math.h"

class Vector 
{
public:
    float x, y, z, w;

    Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
        : x(x), y(y), z(z), w(w) 
	{}

    inline float length() const 
	{
        return std::sqrt( x*x + y*y + z*z);
    }


    inline Vector &normalize() 
	{
		float norm = length();
        if(norm)	invScale(norm);
        return *this;
    }

	inline Vector &scale(float factor) 
	{
        x *= factor;
        y *= factor;
        z *= factor;
        w *= factor;

        return *this;
    }

    inline Vector &invScale(float factor) 
	{
        x /= factor;
        y /= factor;
        z /= factor;
        w /= factor;
        return *this;
    }

    static inline float dot(const Vector &v1, const Vector &v2) 
	{
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }


    static inline Vector cross(const Vector &v1, const Vector &v2)
	{
        return Vector(v1.y * v2.z - v1.z * v2.y,
					v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x);
    }

    ///**
    // * Orthogonal projection of v with relation to w.
    // **/
    //static inline Vector proj(Vector w, const Vector &v) {
    //    float dotProd = dot(v, w);
    //    double magnitude2 = std::pow(w.magnitude(), 2);
    //    double factor = dotProd / magnitude2;

    //    w *= factor;
    //    return w;
    //}

    /// += operator for vectors.
    Vector &operator+=(const Vector &rhv) 
	{
        x += rhv.x;
        y += rhv.y;
        z += rhv.z;
		w += rhv.w;
        return *this;
    }

    Vector &operator-=(const Vector &rhv) 
	{
        x -= rhv.x;
        y -= rhv.y;
        z -= rhv.z;
        w -= rhv.w;
        return *this;
    }

    Vector &operator-=(const float &rhv) 
	{
        x -= rhv;
        y -= rhv;
        z -= rhv;
        return *this;
    }

    Vector &operator*=(const float &factor) 
	{
        this->scale(factor);
        return *this;
    }


    Vector &operator/=(const float &factor) 
	{
        this->invScale(factor);
        return *this;
    }
};


inline Vector operator+(Vector lhv, const Vector &rhv) 
{
    lhv += rhv;
    return lhv;
}

/// - operator for vectors.
inline Vector operator-(Vector lhv, const Vector &rhv) 
{
    lhv -= rhv;
    return lhv;
}

/// - operator for vectors with floats.
inline Vector operator-(Vector lhv, float rhv) 
{
    lhv -= rhv;
    return lhv;
}

/// * operator for scaling vectors.
inline Vector operator*(Vector lhv, const float &rhv)
{
    lhv *= rhv;
    return lhv;
}

/// * operator for scaling vectors.
inline Vector operator*(const float &lhv, Vector rhv)
{
    rhv *= lhv;
    return rhv;
}

/// / operator for scaling vectors.
inline Vector operator/(Vector lhv, const float &rhv)
{
    lhv *= rhv;
    return lhv;
}

/// == operator for vectors.
inline bool operator==(const Vector &lhv, const Vector &rhv)
{
    return (lhv.x == rhv.x && lhv.y == rhv.y && lhv.z == rhv.z && lhv.w == rhv.w);
}

/// != operator for vectors.
inline bool operator!=(const Vector &lhv, const Vector &rhv)
{
    return !operator==(lhv, rhv);
}

/**
 * Operator overloading for writing vectors to output streams.
 **/
inline std::ostream &operator<<(std::ostream &stream, const Vector &vec)
{
    stream << "( " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << " )";
    return stream;
}

#endif 