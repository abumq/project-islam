#ifndef MATHS_H
#define MATHS_H

#include <cmath>

namespace maths {
double dsin(double d) {
    return sin(deg2rad(d));
}

/* degree cos */
double dcos(double d) {
    return cos(deg2rad(d));
}

/* degree tan */
double dtan(double d) {
    return tan(deg2rad(d));
}

/* degree arcsin */
double darcsin(double x) {
    return rad2deg(asin(x));
}

/* degree arccos */
double darccos(double x) {
    return rad2deg(acos(x));
}

/* degree arctan */
double darctan(double x) {
    return rad2deg(atan(x));
}

/* degree arctan2 */
double darctan2(double y, double x) {
    return rad2deg(atan2(y, x));
}

/* degree arccot */
double darccot(double x) {
    return rad2deg(atan(1.0 / x));
}

/* degree to radian */
double deg2rad(double d) {
    return d * M_PI / 180.0;
}

/* radian to degree */
double rad2deg(double r) {
    return r * 180.0 / M_PI;
}

/* range reduce angle in degrees. */
double fix_angle(double a) {
    a = a - 360.0 * floor(a / 360.0);
    a = a < 0.0 ? a + 360.0 : a;
    return a;
}

/* range reduce hours to 0..23 */
double fix_hour(double a) {
    a = a - 24.0 * floor(a / 24.0);
    a = a < 0.0 ? a + 24.0 : a;
    return a;
}
}
#endif // MATHS_H
