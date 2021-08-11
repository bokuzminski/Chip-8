#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

void display_init(Display *display)
{
    SDL_Init(SDL_INIT_VIDEO);

    display->window = SDL_CreateWindow(
        "CHIP-8",               // window label
        SDL_WINDOWPOS_CENTERED, // initial x position
        SDL_WINDOWPOS_CENTERED, // initial y position
        640,
        320,
        SDL_WINDOW_SHOWN // flags
    );
    if (display->window == NULL)
    {
        printf("Could not create SDL Window: %s\n", SDL_GetError());
        exit(1);
    }
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    if (display->renderer == NULL)
    {
        printf("Could not create SDL Renderer: %s\n", SDL_GetError());
        exit(1);
    }
    display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         64,
                                         32);
    if (display->texture == NULL)
    {
        printf("Could not create SDL Texture: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
    SDL_RenderClear(display->renderer);
    SDL_RenderPresent(display->renderer);
}

void display_draw(Display *display, uint32_t pixels[64 * 32])
{
    SDL_UpdateTexture(display->texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(display->renderer);
    SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
    SDL_RenderPresent(display->renderer);
}

void display_cleanup(Display *display)
{
    SDL_DestroyTexture(display->texture);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}