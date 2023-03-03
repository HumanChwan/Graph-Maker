#include <math.h>
#include <stdlib.h>

#include "graph_math.h"
#include "graphics.h"

void destroy_x_equation(XEquation eq) {
    if (eq.terms != NULL) free(eq.terms);
}
void destroy_xy_equation(XYEquation eq) {
    if (eq.terms != NULL) free(eq.terms);
}

XEquation get_y_equation_from_xy(XYEquation eq, float x) {
    XEquation y_eq = {0};
    y_eq.terms = malloc(eq.n * sizeof(Pair));
    y_eq.n = eq.n;

    for (size_t i = 0; i < eq.n; ++i) {
        y_eq.terms[i].cf = eq.terms[i].cf * powf(x, eq.terms[i].x);
        y_eq.terms[i].e = eq.terms[i].y;
    }

    return y_eq;
}

float evaluate_x_equation(XEquation eq, float x) {
    float res = 0.f;

    for (size_t i = 0; i < eq.n; ++i) {
        res += eq.terms[i].cf * powf(x, eq.terms[i].e);
    }

    return res;
}

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
int32_t get_pixel_x_from_coordinate(size_t width, int32_t X, int32_t offset_X, size_t scale) {
    return width / 2 + (X - offset_X) * scale;
}
int32_t get_pixel_y_from_coordinate(size_t height, int32_t Y, int32_t offset_Y, size_t scale) {
    return height / 2 + (offset_Y - Y) * scale;
}
float get_x_coordinate_from_pixel(size_t width, int32_t px, int32_t offset_X, size_t scale) {
    return offset_X + (px - (float)width / 2) / (float)scale;
}
float get_y_coordinate_from_pixel(size_t height, int32_t py, int32_t offset_Y, size_t scale) {
    return offset_Y - (py - (float)height / 2) / (float)scale;
}
Coordinate _coordinate_to_pixel_location(Canvas c, int32_t X, int32_t Y, size_t scale, Coordinate offset) {
    return (Coordinate){ .X = get_pixel_x_from_coordinate(c.width, X, offset.X, scale), 
                         .Y =  get_pixel_y_from_coordinate(c.height, Y, offset.Y, scale) };
}
Coordinate coordinate_to_pixel_location(Canvas c, Coordinate P, size_t scale, Coordinate offset) {
    return _coordinate_to_pixel_location(c, P.X, P.Y, scale, offset);
}

// scale = PIXELS PER UNIT 
void graph_initialize(Canvas canvas, Coordinate center, size_t scale) {
    const RGBAPixel BLACK = {0, 0, 0, 255};
    const RGBAPixel WHITE = {255, 255, 255, 255};
    const RGBAPixel HardGray = {200, 200, 200, 255};
    const RGBAPixel SoftGray = {240, 240, 240, 255};

    fill_canvas(canvas, WHITE);

    // draw axes
    Coordinate origin_pixel = _coordinate_to_pixel_location(canvas, 0, 0, scale, center);
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

    float cx_minus = get_x_coordinate_from_pixel(canvas.width, px - 1, offset.X, scale);
    // float cx = get_x_coordinate_from_pixel(px, offset.X, scale);
    float cx_plus = get_x_coordinate_from_pixel(canvas.width, px + 1, offset.X, scale);

    XEquation y_minus_eq = get_y_equation_from_xy(eq, cx_minus);
    // XEquation y_eq = get_y_equation_from_xy(eq, cx);
    XEquation y_plus_eq = get_y_equation_from_xy(eq, cx_plus);

    // for (size_t i = 0; i < y_eq.n; ++i) {
    //     LOG("(%f, %f)\n", y_eq.terms[i].cf, y_eq.terms[i].e);
    // }

    for (size_t py = 0; py < canvas.height; ++py) {
        float cy_minus = get_y_coordinate_from_pixel(canvas.height, py - 1, offset.Y, scale);
        float cy_plus = get_y_coordinate_from_pixel(canvas.height, py + 1, offset.Y, scale);

        float vert[4] = {
            evaluate_x_equation(y_minus_eq, cy_minus), 
            evaluate_x_equation(y_minus_eq, cy_plus),
            evaluate_x_equation(y_plus_eq, cy_minus), 
            evaluate_x_equation(y_plus_eq, cy_plus)
        };
        
        if (vert[0] * vert[2] <= EPS  || vert[1] * vert[3] <= EPS)
            plot_pixel(canvas, (Coordinate){px, py}, line_width, color);


        // if (ABS(f_y - 0.f) <= EPS) plot_pixel(canvas, (Coordinate){px, py}, line_width, color);
    }

    // destroy_x_equation(y_eq);
    destroy_x_equation(y_minus_eq);
    destroy_x_equation(y_plus_eq);
}

void sketch_graph_for_equation(Canvas canvas, XYEquation eq, Coordinate offset, size_t scale,
        size_t line_width, RGBAPixel color) {

    for (size_t px = 0; px < canvas.width; ++px)
        compute_y_for_x(canvas, eq, px, offset, scale, line_width, color);
}
