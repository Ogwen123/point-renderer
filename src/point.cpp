#include "point.h"

Point::Point() {}
Point::Point(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// rotate around the x axis
Point &Point::rotate_x(double angle)
{
    double s = sin(angle);
    double c = cos(angle);

    double temp_y = this->y;
    double temp_z = this->z;

    this->y = c * temp_y - s * temp_z;
    this->z = s * temp_y + c * temp_z;

    return *this;
}

// rotate around the y axis
Point &Point::rotate_y(double angle)
{
    double s = sin(angle);
    double c = cos(angle);

    double temp_x = this->x;
    double temp_z = this->z;

    this->x = c * temp_x + s * temp_z;
    this->z = -s * temp_x + c * temp_z;

    return *this;
}

// rotate around the z axis
Point &Point::rotate_z(double angle)
{
    double s = sin(angle);
    double c = cos(angle);

    double temp_x = this->x;
    double temp_y = this->y;

    this->x = c * temp_x - s * temp_y;
    this->y = s * temp_x + c * temp_y;

    return *this;
}

SDL_FPoint Point::project(double dist)
{
    SDL_FPoint res;

    res.x = this->x / (this->z + dist);
    res.y = this->y / (this->z + dist);
    return res;
}