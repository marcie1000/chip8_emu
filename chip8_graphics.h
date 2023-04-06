#ifndef CHIP8_GRAPHICS_H
#define CHIP8_GRAPHICS_H

#include <SDL.h>

#define BLACK           (SDL_FALSE)
#define WHITE           (SDL_TRUE)
#define PIXEL_BY_WIDTH  (64)
#define PIXEL_BY_HEIGHT (32)
#define PIXEL_DIM       (8)
#define WIDTH           (PIXEL_BY_WIDTH*PIXEL_DIM)
#define HEIGHT          (PIXEL_BY_HEIGHT*PIXEL_DIM)

typedef SDL_bool s_pixel;

typedef struct s_screen {
    SDL_Window *w;
    SDL_Renderer *r;
    s_pixel pixels[PIXEL_BY_WIDTH][PIXEL_BY_HEIGHT];
    Uint32 pixel_height;
    Uint32 pixel_width;
} s_screen;

extern void clear_screen(s_screen *screen);
extern int initialize_screen(s_screen *screen);
extern void destroy_screen(s_screen *screen);
extern void update_screen(s_screen *screen);
extern void resize_screen(s_screen *screen);


#endif //CHIP8_GRAPHICS_H

