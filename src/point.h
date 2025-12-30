#ifndef POINT_H

#define POINT_H

#include <SDL3/SDL.h>

class Point
{
public:
    float x;
    float y;
    float z;
    Point rotate_x(double angle);
    Point rotate_y(double angle);
    Point rotate_z(double angle);
    SDL_FPoint project(double dist);
};

#endif