#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <math.h>

class Color 
{
public:
	float r, g, b;

    Color(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f)
        : r(_r), g(_g), b(_b) 
	{}

    void clamp() 
	{
		r = std::min(r ,1.0f);
		g = std::min(g ,1.0f);
		b = std::min(b ,1.0f);
    }

	

    Color &operator+=(float other) 
	{
        r += other;
        g += other;
        b += other;
        return *this;
    }

    Color &operator+=(const Color &other) 
	{
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Color &operator*=(float other) 
	{
        r *= other;
        g *= other;
        b *= other;
        return *this;
    }

    Color &operator*=(const Color &other) 
	{
        r *= other.r;
        g *= other.g;
        b *= other.b;
        return *this;
    }
};

inline Color operator+(Color lhv, float rhv)
{
    lhv += rhv;
    return lhv;
}

inline Color operator+(Color lhv, const Color &rhv) 
{
    lhv += rhv;
    return lhv;
}

inline Color operator*(Color lhv, float rhv) 
{
    lhv *= rhv;
    return lhv;
}

inline Color operator*(Color lhv, const Color &rhv) 
{
    lhv *= rhv;
    return lhv;
}

inline std::ostream &operator<<(std::ostream &stream, const Color &color) 
{
    stream << (int) (color.r * 255) << " " << (int) (color.g * 255) << " "<< (int) (color.b * 255) << std::endl;
    return stream;
}

#endif
