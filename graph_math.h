#ifndef GRAPH_MAKER_MATH_
#define GRAPH_MAKER_MATH_

#include <stdint.h>

#include "graphics.h"

#define EPS 1e-4 // epsilon used for fractional comparison

typedef struct { float cf, x, y; } Triple;

typedef struct { float cf, e; } Pair;

typedef struct XYEquation { 
    size_t n;
    Triple* terms; 
} XYEquation;

typedef struct {
    size_t n;
    Pair* terms;
} XEquation;

//-------MATH STUFF--------
// constructors and destructors
XEquation get_y_equation_from_xy(XYEquation, float x); 
void destroy_x_equation(XEquation);
void destroy_xy_equation(XYEquation);

float evaluate_x_equation(XEquation, float x);


//-------DRAWING STUFF--------
int32_t get_pixel_x_from_coordinate(size_t width, int32_t X, int32_t offset_X, size_t scale);
int32_t get_pixel_y_from_coordinate(size_t height, int32_t Y, int32_t offset_Y, size_t scale);
float get_x_coordinate_from_pixel(size_t width, int32_t px, int32_t offset_X, size_t scale);
float get_y_coordinate_from_pixel(size_t height, int32_t py, int32_t offset_Y, size_t scale);
Coordinate _coordinate_to_pixel_location(Canvas c, int32_t X, int32_t Y,
        size_t scale, Coordinate offset);
Coordinate coordinate_to_pixel_location(Canvas c, Coordinate P, size_t scale, Coordinate offset);
void graph_initialize(Canvas canvas, Coordinate center, size_t scale);
void compute_y_for_x(Canvas canvas, XYEquation eq, int32_t px, Coordinate offset, size_t scale, 
        size_t line_width, RGBAPixel color);
void sketch_graph_for_equation(Canvas canvas, XYEquation eq, Coordinate offset, size_t scale,
        size_t line_width, RGBAPixel color);

#endif // GRAPH_MAKER_MATH_
