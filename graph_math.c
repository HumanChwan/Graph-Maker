#include <math.h>
#include <stdlib.h>

#include "graph_math.h"

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
