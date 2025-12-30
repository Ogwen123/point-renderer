#include "point.h"

// rotate around the y axis
Point Point::rotate_x(double angle)
{
    Point res;

    double s = sin(angle);
    double c = cos(angle);

    res.x = x;
    res.y = c * y - s * z;
    res.z = s * y + c * z;

    return res;
}

// rotate around the y axis
Point Point::rotate_y(double angle)
{
    Point res;

    double s = sin(angle);
    double c = cos(angle);

    res.x = c * x - s * z;
    res.y = y;
    res.z = s * x + c * z;

    return res;
}

// rotate around the y axis
Point Point::rotate_z(double angle)
{
    Point res;

    double s = sin(angle);
    double c = cos(angle);

    res.x = c * x - s * y;
    res.y = s * x + c * y;
    res.z = z;

    return res;
}

SDL_FPoint Point::project(double dist)
{
    SDL_FPoint res;

    res.x = x / (z + dist);
    res.y = y / (z + dist);
    return res;
}