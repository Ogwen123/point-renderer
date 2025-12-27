#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define FPS 60
#define POINT_SIZE 30
#define RPS 1

#define FG_R 0x50
#define FG_G 0xff
#define FG_B 0x50

struct Point
{
    float x;
    float y;
    float z;
};

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

Point points[] = {
    Point{0.5, 0.5, 1},
    Point{-0.5, 0.5, 1},
    Point{0.5, -0.5, 1},
    Point{-0.5, -0.5, 1},
    Point{0.5, 0.5, 2},
    Point{-0.5, 0.5, 2},
    Point{0.5, -0.5, 2},
    Point{-0.5, -0.5, 2}};

std::vector<size_t> faces[] = {{0, 1, 2}, {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {0, 2, 6}, {0, 4, 6}, {1, 3, 5}, {5, 3, 7}, {0, 1, 5}, {0, 4, 5}, {3, 7, 6}, {3, 2, 6}};

double angle = 0;
double dist = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
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
    res.x = (point.x + 1) * (WINDOW_WIDTH / 2);
    res.y = (1 - (point.y)) * (WINDOW_HEIGHT / 2);

    // std::cout << point.x << ", " << point.y << ", " << res.x << ", " << res.y << std::endl;

    return res;
}

// rotate around the y axis
Point *rotate_y(Point *points, size_t count, double angle)
{
    Point *rotated = new Point[count];

    for (int i = 0; i < count; i++)
    {
        Point p = points[i];

        Point res;

        double s = sin(angle);
        double c = cos(angle);

        res.x = c * p.x - s * p.z;
        res.y = p.y;
        res.z = s * p.x + c * p.z;

        rotated[i] = res;
    }

    return rotated;
}

SDL_FPoint project(Point point)
{
    SDL_FPoint res;

    res.x = point.x / (point.z + dist);
    res.y = point.y / (point.z + dist);
    return screen(res);
}

SDL_FPoint *project(Point *points, size_t count)
{
    SDL_FPoint *screen_points = new SDL_FPoint[count];

    for (int i = 0; i < count; i++)
    {
        Point p = points[i];
        SDL_FPoint res;

        res.x = p.x / (p.z + dist);
        res.y = p.y / (p.z + dist);
        screen_points[i] = screen(res);
        // std::cout << screen_points[i].x << ", " << screen_points[i].y << std::endl;
    }

    return screen_points;
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

SDL_FRect *point_to_rect(SDL_FPoint *points, size_t count)
{

    SDL_FRect *rects = new SDL_FRect[count];

    for (int i = 0; i < count; i++)
    {
        SDL_FPoint p = points[i];
        SDL_FRect res;

        res.x = p.x - POINT_SIZE / 2;
        res.y = p.y - POINT_SIZE / 2;
        res.h = POINT_SIZE;
        res.w = POINT_SIZE;

        rects[i] = res;
    }

    return rects;
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
    dist += 0.01;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 80, 255, 80, SDL_ALPHA_OPAQUE);

    std::cout << angle << std::endl;

    size_t p_count = SDL_arraysize(points);

    // Point *rotated = rotate_y(points, p_count, angle);
    // size_t ro_count = SDL_arraysize(rotated);
    SDL_FPoint *projected = project(points, p_count);
    size_t pr_count = SDL_arraysize(projected);
    SDL_FRect *rects = point_to_rect(projected, p_count);
    size_t r_count = SDL_arraysize(rects);

    // std::cout << "points: " << p_count << ", rotated: " << ro_count << ", projected: " << pr_count << ", rects: " << r_count << std::endl;

    // draw as points
    // SDL_RenderFillRects(renderer, rects, SDL_arraysize(points));

    // draw as lines to create faces

    // don't draw the same line twice
    std::set<Line> to_draw = {};
    std::cout << (sizeof(faces) / sizeof(faces[0])) << std::endl;
    for (int i = 0; i < (sizeof(faces) / sizeof(faces[0])); i++)
    {
        std::vector<size_t> f = faces[i];

        for (int j = 0; j < f.size(); i++)
        {
            SDL_FPoint start = project(points[f[j]]);
            SDL_FPoint end = project(points[f[(j + 1) % f.size()]]);

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
