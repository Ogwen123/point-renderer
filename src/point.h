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
    void rotate_x(double angle);
    void rotate_y(double angle);
    void rotate_z(double angle);
    SDL_FPoint project(double dist);
};

#endif