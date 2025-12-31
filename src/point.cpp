#include "point.h"

Point::Point() {}
Point::Point(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// rotate around the y axis
Point Point::rotate_x(double angle)
{
    Point res;

    double s = sin(angle);
    double c = cos(angle);

    res.x = this->x;
    res.y = c * this->y - s * this->z;
    res.z = s * this->y + c * this->z;

    return res;
}

// rotate around the y axis
Point Point::rotate_y(double angle)
{
    Point res;

    double s = sin(angle);
    double c = cos(angle);

    res.x = c * this->x - s * this->z;
    res.y = this->y;
    res.z = s * this->x + c * this->z;

    return res;
}

// rotate around the y axis
Point Point::rotate_z(double angle)
{
    Point res;

    double s = sin(angle);
    double c = cos(angle);

    res.x = c * this->x - s * this->y;
    res.y = s * this->x + c * this->y;
    res.z = this->z;

    return res;
}

SDL_FPoint Point::project(double dist)
{
    SDL_FPoint res;

    res.x = this->x / (this->z + dist);
    res.y = this->y / (this->z + dist);
    return res;
}