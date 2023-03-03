#include <stdio.h>
#include <stdlib.h>

#define LOG(...) fprintf(stderr, "LOG: "__VA_ARGS__)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "graphics.h"
#include "graph_math.h"

#define SCREEN_FACTOR 150
#define WIDTH (16*SCREEN_FACTOR)
#define HEIGHT (9*SCREEN_FACTOR)

#define EPS 1e-1 // epsilon used for fractional comparison
                
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

const RGBAPixel HardGray = {200, 200, 200, 255};
const RGBAPixel SoftGray = {240, 240, 240, 255};


// What is scale??
//
// Should it be pixels per unit
// or units per pixel
//
// PIXELS PER UNIT seems logical hmmm
// 
//
// Should be Integer-ish. :thinking:
//

// TODOOOO: Remove dependence on global define HEIGHT and WIDTH
int32_t get_pixel_x_from_coordinate(int32_t X, int32_t offset_X, size_t scale) {
    return WIDTH / 2 + (X - offset_X) * scale;
}
int32_t get_pixel_y_from_coordinate(int32_t Y, int32_t offset_Y, size_t scale) {
    return HEIGHT / 2 + (offset_Y - Y) * scale;
}
float get_x_coordinate_from_pixel(int32_t px, int32_t offset_X, size_t scale) {
    return offset_X + (px - (float)WIDTH / 2) / (float)scale;
}
float get_y_coordinate_from_pixel(int32_t py, int32_t offset_Y, size_t scale) {
    return offset_Y - (py - (float)HEIGHT / 2) / (float)scale;
}
Coordinate _coordinate_to_pixel_location(int32_t X, int32_t Y, size_t scale, Coordinate offset) {
    return (Coordinate){ .X = get_pixel_x_from_coordinate(X, offset.X, scale), 
                         .Y =  get_pixel_y_from_coordinate(Y, offset.Y, scale) };
}
Coordinate coordinate_to_pixel_location(Coordinate P, size_t scale, Coordinate offset) {
    return _coordinate_to_pixel_location(P.X, P.Y, scale, offset);
}

// scale = PIXELS PER UNIT 
void graph_initialize(Canvas canvas, Coordinate center, size_t scale) {
    const RGBAPixel BLACK = {0, 0, 0, 255};
    const RGBAPixel WHITE = {255, 255, 255, 255};

    fill_canvas(canvas, WHITE);

    // draw axes
    Coordinate origin_pixel = _coordinate_to_pixel_location(0, 0, scale, center);
    // Coordinate center_pixel = coordinate_to_pixel_location(center, scale, center);

    float frac_increment = (float)scale / 10.f;
    for (size_t x = origin_pixel.X % scale; x < canvas.width; x += scale) {
        for (float x0 = x - scale + frac_increment; x0 < x; x0 += frac_increment)
            draw_line(canvas, (Coordinate){.X = x0, .Y = 0}, (Coordinate){
                    .X = x0, .Y = canvas.height - 1 }, 1, SoftGray);
    }

    for (size_t y = origin_pixel.Y % scale; y < canvas.height; y += scale) {
        for (float y0 = y - scale + frac_increment; y0 < y; y0 += frac_increment)
            draw_line(canvas, (Coordinate){.X = 0, .Y = y0}, (Coordinate){
                    .X = canvas.width - 1, .Y = y0 }, 1, SoftGray);

        draw_line(canvas, (Coordinate){.X = 0, .Y = y}, (Coordinate){
                .X = canvas.width - 1, .Y = y }, 2, HardGray);
    }
    for (size_t x = origin_pixel.X % scale; x < canvas.width; x += scale) {
        draw_line(canvas, (Coordinate){.X = x, .Y = 0}, (Coordinate){
                .X = x, .Y = canvas.height - 1 }, 2, HardGray);
    }

    // X-axis
    draw_line(canvas, (Coordinate){.X = 0, .Y = origin_pixel.Y}, (Coordinate){
            .X = canvas.width - 1, .Y = origin_pixel.Y }, 3, BLACK);
    // Y-axis
    draw_line(canvas, (Coordinate){.X = origin_pixel.X, .Y = 0}, (Coordinate){
            .X = origin_pixel.X, .Y = canvas.height - 1 }, 3, BLACK);

    // origin
    draw_filled_circle(canvas, origin_pixel, 5, BLACK);
} 


// scale = PIXELS PER UNIT 
void compute_y_for_x(Canvas canvas, XYEquation eq, int32_t px, Coordinate offset, size_t scale, 
        size_t line_width, RGBAPixel color) {

    float cx = get_x_coordinate_from_pixel(px, offset.X, scale);

    XEquation y_eq = get_y_equation_from_xy(eq, cx);

    // for (size_t i = 0; i < y_eq.n; ++i) {
    //     LOG("(%f, %f)\n", y_eq.terms[i].cf, y_eq.terms[i].e);
    // }

    for (size_t py = 0; py < canvas.height; ++py) {
        float cy = get_y_coordinate_from_pixel(py, offset.Y, scale);
        float f_y = evaluate_x_equation(y_eq, cy);

        if (ABS(f_y - 0.f) <= EPS) plot_pixel(canvas, (Coordinate){px, py}, line_width, color);
    }

    destroy_x_equation(y_eq);
}

void sketch_graph_for_equation(Canvas canvas, XYEquation eq, Coordinate offset, size_t scale,
        size_t line_width, RGBAPixel color) {

    for (size_t px = 0; px < canvas.width; ++px)
        compute_y_for_x(canvas, eq, px, offset, scale, line_width, color);
}


int main(void) {
    size_t scale = HEIGHT / 25;
    Coordinate center = {5, 5};
    graph_initialize(canvas, center, scale);

    XYEquation eq;
    Triple terms[2] = {{1, .x=0, .y=1}, {-1, .x=2, .y=0}};
    eq.terms = terms;
    eq.n = 2;

    const RGBAPixel BLUE = {0, 0, 255, 255};
    sketch_graph_for_equation(canvas, eq, center, scale, 1, BLUE);

    save_canvas_as_png(canvas, "images\\test.png");
    return 0;
}
