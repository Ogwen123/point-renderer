#ifndef POINT_H

#define POINT_H

#include <SDL3/SDL.h>
#include <cmath>

class Point
{
private:
    float x;
    float y;
    float z;

public:
    Point();
    Point(double x, double y, double z);
    Point &rotate_x(double angle);
    Point &rotate_y(double angle);
    Point &rotate_z(double angle);
    SDL_FPoint project(double dist);
    float get_x() const { return x; }
    float get_y() const { return y; }
    float get_z() const { return z; }
};

#endif