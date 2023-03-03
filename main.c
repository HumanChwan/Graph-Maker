#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG(...) fprintf(stderr, "LOG: "__VA_ARGS__)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "graph_math.h"

#define SCREEN_FACTOR 150
#define WIDTH (16*SCREEN_FACTOR)
#define HEIGHT (9*SCREEN_FACTOR)

#define ABS(x) ((x) > 0 ? (x) : -(x))

// saving functions
void save_canvas_as_png(Canvas c, const char* file_path) {
    char *data = malloc(c.width * c.height * 3);  

    for (size_t i = 0; i < c.width * c.height; ++i)
        data[3 * i] = c.pixels[i].R, 
            data[3 * i + 1] = c.pixels[i].G, 
            data[3 * i + 2] = c.pixels[i].B;

    stbi_write_png(file_path, c.width, c.height, 3, data, 3 * c.stride);

    free(data);
}

static RGBAPixel pixels[HEIGHT*WIDTH] = {0};

static Canvas canvas = {
    .width=WIDTH,
    .height=HEIGHT,
    .stride=WIDTH,
    .pixels=pixels
};


void draw_circle_at_origin(Coordinate center, size_t scale) {
    XYEquation eq;
    Triple terms[3] = {{1, .x=2, .y=0}, {1, .x=0, .y=2}, {-100, .x=0, .y=0}};
    eq.terms = terms;
    eq.n = 3;

    const RGBAPixel GREEN = {0, 150, 0, 255};
    sketch_graph_for_equation(canvas, eq, center, scale, 3, GREEN);
}

void draw_circle(Coordinate center, size_t scale) {
    XYEquation eq;
    Triple terms[5] = {{1.f/100.f, .x=2, .y=0}, {1.f/100.f, .x=0, .y=2}, 
        {-1, .x=0, .y=0}, {-1.f/10.f, .x=1, .y=0}, {-1.f/10.f, .x=0, .y=1}};
    eq.terms = terms;
    eq.n = 5;

    const RGBAPixel GREEN = {0, 150, 0, 255};
    sketch_graph_for_equation(canvas, eq, center, scale, 3, GREEN);
}

void draw_quadratic_equation(Coordinate center, size_t scale) {
    XYEquation eq;
    Triple terms[2] = {{1, .x=0, .y=1}, {-1, .x=2, .y=0}};
    eq.terms = terms;
    eq.n = 2;

    const RGBAPixel BLUE = {0, 0, 255, 255};

    sketch_graph_for_equation(canvas, eq, center, scale, 1, BLUE);
}

void draw_ellipse(Coordinate center, size_t scale) {
    XYEquation eq;
    Triple terms[3] = {{1.f/36.f, .x=0, .y=2}, {1.f/100.f, .x=2, .y=0}, {-1, .x=0, .y=0}};
    eq.terms = terms;
    eq.n = 3;

    const RGBAPixel RED = {255, 0, 0, 255};
    sketch_graph_for_equation(canvas, eq, center, scale, 1, RED);
}


int main(void) {
    size_t scale = HEIGHT / 25;
    Coordinate center = {5, 5};
    graph_initialize(canvas, center, scale);

    draw_quadratic_equation(center, scale);

    save_canvas_as_png(canvas, "images\\test.png");
    return 0;
}
