#ifndef GRAPH_MAKER_MATH_
#define GRAPH_MAKER_MATH_

#include <stdint.h>

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

// constructors and destructors
XEquation get_y_equation_from_xy(XYEquation, float x); 
void destroy_x_equation(XEquation);
void destroy_xy_equation(XYEquation);

float evaluate_x_equation(XEquation, float x);

#endif // GRAPH_MAKER_MATH_
