#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

int display_init(Display *display)
{
    SDL_Init(SDL_INIT_VIDEO);

    display->window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 320);

    return 0;
}

void display_draw(Display *display, uint32_t pixels[64][32])
{
    SDL_UpdateTexture(display->texture, NULL, pixels, 640 * sizeof(uint32_t));
    SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
    SDL_RenderPresent(display->renderer);
}