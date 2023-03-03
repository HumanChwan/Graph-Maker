#ifndef GRAPH_MAKER_GRAPHICS_
#define GRAPH_MAKER_GRAPHICS_

#include <stdint.h>
#include <stdbool.h>

#define SWAP(X, Y, type) do {   \
    type temp = X;              \
    X = Y;                      \
    Y = temp;                   \
} while (0);

typedef struct  {
    uint8_t R, G, B, A;
} RGBAPixel;

typedef struct Canvas {
    size_t width, height; 
    size_t stride;
    RGBAPixel* pixels;
} Canvas;

typedef struct Coordinate {
    int32_t X, Y;
} Coordinate;

// Util functions
RGBAPixel blend_color(RGBAPixel back, RGBAPixel fore);
bool valid_coordinate(Canvas canvas, Coordinate P);

// Canvas methods
void plot_pixel(Canvas canvas, Coordinate P, size_t line_width, RGBAPixel color);
void fill_canvas(Canvas canvas, RGBAPixel color);
void draw_line(Canvas canvas, Coordinate A, Coordinate B, size_t line_width, RGBAPixel color);
void draw_filled_circle(Canvas canvas, Coordinate P, size_t radius, RGBAPixel color);

#endif // GRAPH_MAKER_GRAPHICS_
