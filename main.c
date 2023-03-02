#include <stdio.h>
#include <stdlib.h>

#define LOG(...) fprintf(stderr, "LOG: "__VA_ARGS__)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "graphics.h"

#define SCREEN_FACTOR 150
#define WIDTH (16*SCREEN_FACTOR)
#define HEIGHT (9*SCREEN_FACTOR)

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

const RGBAPixel BLACK = {0, 0, 0, 255};
const RGBAPixel WHITE = {255, 255, 255, 255};
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
Coordinate _coordinate_to_pixel_location(int32_t X, int32_t Y, size_t scale, Coordinate offset) {
    return (Coordinate){ .X = WIDTH / 2 + (X - offset.X) * scale, 
                         .Y = HEIGHT / 2 + (offset.Y - Y) * scale };
}
Coordinate coordinate_to_pixel_location(Coordinate P, size_t scale, Coordinate offset) {
    return _coordinate_to_pixel_location(P.X, P.Y, scale, offset);
}

// scale = PIXELS PER UNIT 
void graph_initialize(Canvas canvas, Coordinate center, size_t scale) {
    fill_canvas(canvas, WHITE);
    // draw axes
    Coordinate origin_pixel = _coordinate_to_pixel_location(0, 0, scale, center);
    Coordinate center_pixel = coordinate_to_pixel_location(center, scale, center);
    LOG("Origin: {%d, %d}, Scale: %zu\n", origin_pixel.X, origin_pixel.Y, scale);
    LOG("Center: {%d, %d}\n", center_pixel.X, center_pixel.Y);

    float frac_increment = (float)scale / 10.f;
    for (int x = origin_pixel.X % scale; x < WIDTH; x += scale) {
        for (float x0 = x - scale + frac_increment; x0 < x; x0 += frac_increment)
            draw_line(canvas, (Coordinate){.X = x0, .Y = 0}, (Coordinate){
                    .X = x0, .Y = HEIGHT - 1 }, 1, SoftGray);
    }

    for (int y = origin_pixel.Y % scale; y < HEIGHT; y += scale) {
        for (float y0 = y - scale + frac_increment; y0 < y; y0 += frac_increment)
            draw_line(canvas, (Coordinate){.X = 0, .Y = y0}, (Coordinate){
                    .X = WIDTH - 1, .Y = y0 }, 1, SoftGray);

        draw_line(canvas, (Coordinate){.X = 0, .Y = y}, (Coordinate){
                .X = WIDTH - 1, .Y = y }, 2, HardGray);
    }
    for (int x = origin_pixel.X % scale; x < WIDTH; x += scale) {
        draw_line(canvas, (Coordinate){.X = x, .Y = 0}, (Coordinate){
                .X = x, .Y = HEIGHT - 1 }, 2, HardGray);
    }

    // X-axis
    draw_line(canvas, (Coordinate){.X = 0, .Y = origin_pixel.Y}, (Coordinate){
            .X = WIDTH - 1, .Y = origin_pixel.Y }, 3, BLACK);
    // Y-axis
    draw_line(canvas, (Coordinate){.X = origin_pixel.X, .Y = 0}, (Coordinate){
            .X = origin_pixel.X, .Y = HEIGHT - 1 }, 3, BLACK);
} 

int main(void) {
    graph_initialize(canvas, (Coordinate){5, 5}, HEIGHT / 25);

    save_canvas_as_png(canvas, "images\\test.png");
    return 0;
}
