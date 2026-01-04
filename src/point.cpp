#include "point.h"

Point::Point() {}
Point::Point(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// rotate around the y axis
void Point::rotate_x(double angle)
{
    double s = sin(angle);
    double c = cos(angle);

    this->x = this->x;
    this->y = c * this->y - s * this->z;
    this->z = s * this->y + c * this->z;
}

// rotate around the y axis
void Point::rotate_y(double angle)
{
    double s = sin(angle);
    double c = cos(angle);

    this->x = c * this->x - s * this->z;
    this->y = this->y;
    this->z = s * this->x + c * this->z;
}

// rotate around the y axis
void Point::rotate_z(double angle)
{
    double s = sin(angle);
    double c = cos(angle);

    this->x = c * this->x - s * this->y;
    this->y = s * this->x + c * this->y;
    this->z = this->z;
}

SDL_FPoint Point::project(double dist)
{
    SDL_FPoint res;

    res.x = this->x / (this->z + dist);
    res.y = this->y / (this->z + dist);
    return res;
}