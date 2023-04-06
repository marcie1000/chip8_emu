#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "chip8_graphics.h"

void clear_screen(s_screen *screen)
{
    memset(screen->pixels, BLACK, sizeof(screen->pixels));
}

int initialize_screen(s_screen *screen)
{
    screen->w = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                 WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(NULL == screen->w) 
    {
        fprintf(stderr, "Error SDL_CreateWindow: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    screen->r = SDL_CreateRenderer(screen->w, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == screen->r)
    {
        fprintf(stderr, "Error SDL_CreateRenderer: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    clear_screen(screen);
    screen->pixel_width = PIXEL_DIM;
    screen->pixel_height = PIXEL_DIM;
    
    return EXIT_SUCCESS;
}

void destroy_screen(s_screen *screen)
{
    if(screen->r != NULL)
        SDL_DestroyRenderer(screen->r);
    if(screen->w != NULL)
        SDL_DestroyWindow(screen->w);
}

void update_screen(s_screen *screen)
{
    SDL_SetRenderDrawColor(screen->r, 0, 0, 0, 255);
    SDL_RenderClear(screen->r);
    
    SDL_Rect pixel = {0, 0, screen->pixel_width, screen->pixel_height};
    SDL_SetRenderDrawColor(screen->r, 255, 255, 255, 255);
    //calculates coordinates in the window for each chip8 pixel and show them
    //ord
    for(size_t absc = 0; absc < PIXEL_BY_WIDTH; absc++)
    {
        //absc
        for(size_t ord = 0; ord < PIXEL_BY_HEIGHT; ord++)
        {
            if(screen->pixels[absc][ord] == WHITE)
            {
                pixel.x = absc * screen->pixel_width;
                pixel.y = ord * screen->pixel_height;
                SDL_RenderFillRect(screen->r, &pixel);
            }
        }
    }
    SDL_RenderPresent(screen->r);
}

void resize_screen(s_screen *screen)
{
    int w, h;
    SDL_GetWindowSize(screen->w, &w, &h);
    
    screen->pixel_width = w / PIXEL_BY_WIDTH;
    screen->pixel_height = h / PIXEL_BY_HEIGHT;
}

