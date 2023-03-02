#include "graphics.h"

#ifdef DEBUG
#include <stdio.h>
#define LOG(...) fprintf(stderr, "LOG: "__VA_ARGS__)
#else
#define LOG(...)
#endif


void plot_pixel(Canvas canvas, Coordinate P, size_t line_width, RGBAPixel color) {
    if (valid_coordinate(canvas, P))
        canvas.pixels[P.Y * canvas.stride + P.X] = color;

    int16_t radius = line_width / 2;
    for (int16_t y = -radius; y <= radius; ++y) {
        for (int16_t x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                Coordinate c = {x + P.X, y + P.Y};
                if (valid_coordinate(canvas, c))
                    canvas.pixels[c.Y * canvas.stride + c.X] = color;
            }
        }
    }
}

void draw_line_with_g_slope(Canvas canvas, Coordinate A, Coordinate B, size_t line_width, RGBAPixel color) {
    Coordinate delta = { .X = B.X - A.X, .Y = B.Y - A.Y };
    Coordinate increment = { .Y = 1, .X = (delta.X > 0 ? 1 : -1) };
    if (delta.X < 0) delta.X *= -1;

    int16_t d = 2 * delta.X - delta.Y;

    for (int32_t x = A.X, y = A.Y; y <= B.Y; y += increment.Y) {
        plot_pixel(canvas, (Coordinate){x, y}, line_width, color);
        if (d > 0) {
            x += increment.X;
            d += 2 * (delta.X - delta.Y);
        } else d += 2 * delta.X;
    }
}
void draw_line_with_l_slope(Canvas canvas, Coordinate A, Coordinate B, size_t line_width, RGBAPixel color) {
    Coordinate delta = { .X = B.X - A.X, .Y = B.Y - A.Y };
    Coordinate increment = { .X = 1, .Y = (delta.Y > 0 ? 1 : -1) };
    if (delta.Y < 0) delta.Y *= -1;

    int16_t d = 2 * delta.Y - delta.X;

    for (int32_t x = A.X, y = A.Y; x <= B.X; x += increment.X) {
        plot_pixel(canvas, (Coordinate){x, y}, line_width, color);
        if (d > 0) {
            y += increment.Y;
            d += 2 * (delta.Y - delta.X);
        } else d += 2 * delta.Y;
    }
}

// Canvas methods
void fill_canvas(Canvas c, RGBAPixel color) {
    for (size_t i = 0; i < c.width * c.height; ++i)
        c.pixels[i] = color;
}
void draw_line(Canvas canvas, Coordinate A, Coordinate B, size_t line_width, RGBAPixel color) {
    // Bresenham line drawing algorithm
    Coordinate delta = {.X = B.X - A.X, .Y = B.Y - A.Y };
    if (delta.X < 0) delta.X *= -1;
    if (delta.Y < 0) delta.Y *= -1;
    // slope division as slope: 
    //      - [-1, 1] 
    //      - (-inf, 1) U (1, inf)
    if (delta.Y > delta.X) {
        if (A.Y > B.Y) SWAP(A, B, Coordinate);
        draw_line_with_g_slope(canvas, A, B, line_width, color);
    } else {
        if (A.X > B.X) SWAP(A, B, Coordinate);
        draw_line_with_l_slope(canvas, A, B, line_width, color);
    }
}

// Util functions

/******************/
// c = a * C
//
// a_o = a_a * f_a + a_b * f_b
//
// c_o = (c_a * f_a + c_b * f_b) / (a_a * f_a + a_b * f_b)
// c_o = (a_a * C_a * f_a + a_b * C_b * f_b) / a_o
//
// C = c / a if a != 0
//
// for over: f_a = 1 and f_b = 1 - a_a
/******************/
RGBAPixel blend_color(RGBAPixel B, RGBAPixel A) {
    RGBAPixel composite;
    
    composite.A = (A.A * 255 + B.A * (255 - A.A)) / 255;

    composite.R = (A.A * A.R + B.A * B.R * (255 - A.A)) / (255 * composite.A);
    composite.G = (A.A * A.G + B.A * B.G * (255 - A.A)) / (255 * composite.A);
    composite.B = (A.A * A.B + B.A * B.B * (255 - A.A)) / (255 * composite.A);

    return composite;
}

bool valid_coordinate(Canvas canvas, Coordinate P) {
    return (0 <= P.X && P.X < (int)canvas.width && 0 <= P.Y && P.Y < (int)canvas.height);
}
