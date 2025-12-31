#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>
#include <string>
#include <tuple>
#include <stdexcept>

#include "obj.h"
#include "point.h"
#include "types.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define FPS 60
#define POINT_SIZE 30
#define RPS 0.2

#define FG_R 0x50
#define FG_G 0xff
#define FG_B 0x50

struct Line
{
    SDL_FPoint start;
    SDL_FPoint end;
};

bool operator<(const Line &lhs, const Line &rhs)
{
    if (lhs.start.x != rhs.start.x)
        return lhs.start.x < rhs.start.x;
    if (lhs.start.y != rhs.start.y)
        return lhs.start.y < rhs.start.y;
    if (lhs.end.x != rhs.end.x)
        return lhs.end.x < rhs.end.x;
    return lhs.end.y < rhs.end.y;
}

std::vector<Point> points = {
    Point{0.5, 0.5, -0.5},
    Point{-0.5, 0.5, -0.5},
    Point{0.5, -0.5, -0.5},
    Point{-0.5, -0.5, -0.5},
    Point{0.5, 0.5, 0.5},
    Point{-0.5, 0.5, 0.5},
    Point{0.5, -0.5, 0.5},
    Point{-0.5, -0.5, 0.5}};

// std::vector<size_t> faces[] = {{0, 1, 2}, {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {0, 2, 6}, {0, 4, 6}, {1, 3, 5}, {5, 3, 7}, {0, 1, 5}, {0, 4, 5}, {3, 7, 6}, {3, 2, 6}};
// std::vector<Face> faces = {
//     {1, 0},
//     {1, 3},
//     {1, 5},
//     {0, 4},
//     {0, 2},
//     {2, 6},
//     {2, 3},
//     {5, 4},
//     {4, 6},
//     {6, 7},
//     {7, 5},
//     {3, 7}};

std::vector<Face> faces = {{1, 0, 2, 3}, {0, 4, 6, 2}, {5, 1, 3, 7}, {5, 4, 0, 1}, {3, 2, 6, 7}, {5, 4, 6, 7}};

double angle = 0;
double dist = 3;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    std::string path = "";
    ObjData d;
    if (argc == 2)
    {
        path = argv[1];

        try
        {
            d = ObjData(path);
        }
        catch (std::invalid_argument &e)
        {
            std::cout << "[ERROR] " << e.what() << std::endl;
            return SDL_APP_FAILURE;
        }
        catch (...)
        {
            std::cout << "Encountered unspecified error" << std::endl;
        }

        points = d.vertex_data();
        faces = d.faces;

        std::cout << points.size() << std::endl;

        std::cout << "Finished loading .obj" << std::endl;
    }

    int i;

    SDL_SetAppMetadata("Simple 3D Renderer", "1.0", "org.eu.ogwen.point-renderer");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Simple 3D Renderer", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;
}

// turn the unit coords the points are defined in to the screen coords
SDL_FPoint screen(SDL_FPoint point)
{
    SDL_FPoint res;
    res.x = ((point.x + 1) / 2) * WINDOW_WIDTH;
    res.y = (1 - (point.y + 1) / 2) * WINDOW_HEIGHT;

    // std::cout << point.x << ", " << point.y << ", " << res.x << ", " << res.y << std::endl;

    return res;
}

SDL_FRect point_to_rect(SDL_FPoint p)
{
    SDL_FRect res;

    res.x = p.x - POINT_SIZE / 2;
    res.y = p.y - POINT_SIZE / 2;
    res.h = POINT_SIZE;
    res.w = POINT_SIZE;

    return res;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 start = SDL_GetTicks();

    angle += ((SDL_PI_D * 2) / FPS) * RPS;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 80, 255, 80, SDL_ALPHA_OPAQUE);

    size_t p_count = SDL_arraysize(points);

    // draw triangles from the faces array

    // don't draw the same line twice
    std::set<Line> to_draw = {};

    for (auto f : faces)
    {
        for (int j = 0; j < f.size(); j++)
        {

            Point start_point = points[f[j]];
            Point end_point = points[f[(j + 1) % f.size()]];

            SDL_FPoint start = screen(start_point.rotate_x(angle).project(dist));
            SDL_FPoint end = screen(end_point.rotate_x(angle).project(dist));

            Line res;

            res.start = start;
            res.end = end;

            to_draw.insert(res);
        }
    }
    for (Line line : to_draw)
    {
        SDL_RenderLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
    }

    SDL_RenderPresent(renderer);
    Uint64 end = SDL_GetTicks();

    SDL_Delay((1000 / FPS) - (end - start));

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
