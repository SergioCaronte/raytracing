#ifndef MATH_MATH_H
#define MATH_MATH_H

/**
 * Proper inclusion of the standard math library and definition of additional
 * types in case they are missing.
 * Please use this header instead of including <cmath>.
 **/

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

/**
 * Converts from degrees to radians.
 **/
inline double toRads(double degrees) {
    return degrees * (M_PI / 180.0);
}

/**
 * Converts from radians to degrees.
 **/
inline double toDegrees(double rads) {
    return rads * (180.0 / M_PI);
}

#endif // !MATH_MATH_HPP
