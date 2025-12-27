#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <iostream>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define FPS 60
#define POINT_SIZE 30

#define FG_R 0x50
#define FG_G 0xff
#define FG_B 0x50

struct Point
{
    float x;
    float y;
    float z;
};

Point points[] = {Point{0.5, 0.5, 1}, Point{-0.5, 0.5, 1}, Point{0.5, -0.5, 1}, Point{-0.5, -0.5, 1}, Point{0.5, 0.5, 2}, Point{-0.5, 0.5, 2}, Point{0.5, -0.5, 2}, Point{-0.5, -0.5, 2}};

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

SDL_FPoint *project(Point *points, size_t count)
{
    SDL_FPoint *screen_points = new SDL_FPoint[count];

    for (int i = 0; i < count; i++)
    {
        Point p = points[i];
        SDL_FPoint res;

        res.x = p.x / p.z;
        res.y = p.y / p.z;
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

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 80, 255, 80, SDL_ALPHA_OPAQUE);

    size_t p_count = SDL_arraysize(points);
    SDL_FRect *rects = point_to_rect(project(points, p_count), p_count);
    size_t r_count = SDL_arraysize(rects);

    SDL_RenderFillRects(renderer, rects, SDL_arraysize(points));

    SDL_RenderPresent(renderer);
    Uint64 end = SDL_GetTicks();

    SDL_Delay((1000 / FPS) - (end - start));

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}