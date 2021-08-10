#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

void display_init(Display *display)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    display->window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
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
    display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 320);
    if (display->texture == NULL)
    {
        printf("Could not create SDL Texture: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
    SDL_RenderClear(display->renderer);
    SDL_RenderPresent(display->renderer);
}

void buffer_graphics(Chip8 *ch8, uint32_t *buffer, SDL_Renderer *renderer)
{
    for (int y = 0; y < 320; y++)
    {
        for (int x = 0; x < 640; x++)
        {
            uint8_t pixel = ch8->video[(y + x) * 64];
            buffer[(y * 640) + x] = (0xFFFFFF00 * pixel) | 0x000000FF;
        }
    }
}

void display_draw(Display *display, uint32_t *buffer)
{
    SDL_UpdateTexture(display->texture, NULL, buffer, 640 * sizeof(uint32_t));
    SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
    SDL_RenderPresent(display->renderer);
}