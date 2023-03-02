#include <stdio.h>
#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "graphics.h"

#define SCREEN_FACTOR 50
#define WIDTH (16*SCREEN_FACTOR)
#define HEIGHT (9*SCREEN_FACTOR)

// saving functions
void save_canvas_as_png(Canvas c, const char* file_path) {
    char data[c.width * c.height * 3];  

    for (size_t i = 0; i < c.width * c.height; ++i)
        data[3 * i] = c.pixels[i].R, 
            data[3 * i + 1] = c.pixels[i].G, 
            data[3 * i + 2] = c.pixels[i].B;

    stbi_write_png(file_path, c.width, c.height, 3, data, 3 * c.stride);
}

static RGBAPixel pixels[HEIGHT*WIDTH] = {0};

static Canvas canvas = {
    .width=WIDTH,
    .height=HEIGHT,
    .stride=WIDTH,
    .pixels=pixels
};

const RGBAPixel BLACK = {0, 0, 0, 255};

const RGBAPixel RED = {255, 0, 0, 255};
const RGBAPixel GREEN = {0, 255, 0, 255};
const RGBAPixel BLUE = {0, 0, 255, 255};
const RGBAPixel WHITE = {255, 255, 255, 255};

int main(void) {
    fill_canvas(canvas, BLACK);

    draw_line(canvas, (Coordinate){25, 25}, (Coordinate){400, 400}, 10, BLUE);
    draw_line(canvas, (Coordinate){400, 100}, (Coordinate){25, 400}, 2, GREEN);
    draw_line(canvas, (Coordinate){60, 60}, (Coordinate){100, 350}, 15, WHITE);
    draw_line(canvas, (Coordinate){750, 60}, (Coordinate){700, 350}, 15, RED);

    save_canvas_as_png(canvas, "images\\test.png");
    return 0;
}
