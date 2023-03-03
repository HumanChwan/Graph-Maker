#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "graph_math.h"

#define SCREEN_FACTOR 50
#define WIDTH (16*SCREEN_FACTOR)
#define HEIGHT (9*SCREEN_FACTOR)


void handle_exit(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture) {
    if (window) SDL_DestroyWindow(window);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (texture) SDL_DestroyTexture(texture);

    fprintf(stderr, "ERROR: %s\n", SDL_GetError());

    exit(0);
}

static RGBAPixel data[HEIGHT * WIDTH];

Canvas draw_circle(Coordinate center, size_t scale) {
    Canvas c = {.width=WIDTH, .height=HEIGHT, .pixels=data, .stride=WIDTH};
    graph_initialize(c, center, scale);

    XYEquation eq;
    Triple terms[5] = {{1.f/100.f, .x=2, .y=0}, {1.f/100.f, .x=0, .y=2}, 
        {-1, .x=0, .y=0}, {-1.f/10.f, .x=1, .y=0}, {-1.f/10.f, .x=0, .y=1}};
    eq.terms = terms;
    eq.n = 5;

    const RGBAPixel GREEN = {0, 150, 0, 255};
    sketch_graph_for_equation(c, eq, center, scale, 3, GREEN);

    return c;
}

int main(void) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    
    int res = SDL_Init(SDL_INIT_VIDEO);
    if (res < 0) handle_exit(NULL, NULL, NULL);

    window = SDL_CreateWindow("Graph Maker", 10, 50, WIDTH, HEIGHT, 0);
    if (window == NULL) handle_exit(window, NULL, NULL);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) handle_exit(window, renderer, NULL);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, 
            SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (texture == NULL) handle_exit(window, renderer, texture);

    Coordinate center = {5, 5};
    size_t scale = HEIGHT / 25;

    Uint32 prev = SDL_GetTicks();
    while (1) {
        Uint32 curr = SDL_GetTicks();
        float dt = (curr - prev) / 1000.f;
        (void) dt;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    handle_exit(window, renderer, texture);
                } break;
                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_w:{
                            center.Y -= 1;
                        } break;
                        case SDLK_s:{
                            center.Y += 1;
                        } break;
                        case SDLK_a:{
                            center.X += 1;
                        } break;
                        case SDLK_d:{
                            center.X -= 1;
                        } break;
                        case SDLK_j:{
                            scale += 1;
                        } break;
                        case SDLK_k:{
                            scale -= 1;
                        } break;
                    }
                } break;
            }
        }

        SDL_Rect window_rect = {0, 0, WIDTH, HEIGHT};
        Canvas c = draw_circle(center, scale);

        void *pixels_data;
        int pitch;
        if (SDL_LockTexture(texture, &window_rect, &pixels_data, &pitch) < 0)
            handle_exit(window, renderer, texture);
        {
            for (size_t y = 0; y < c.height; ++y)
                memcpy((char*)pixels_data + y*pitch, c.pixels + y*c.width,
                        c.width*sizeof(uint32_t));
        }
        SDL_UnlockTexture(texture);

        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) < 0) handle_exit(window, renderer, texture);

        if (SDL_RenderClear(renderer) < 0) handle_exit(window, renderer, texture);

        if (SDL_RenderCopy(renderer, texture, &window_rect, &window_rect) < 0)
            handle_exit(window, renderer, texture);

        SDL_RenderPresent(renderer);
    }
    return 0;
}
