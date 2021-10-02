#include <unistd.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

#undef main

int main(int argc, char *argv[])
{
    Chip8 ch8;
    chip8_initialization(&ch8);
    FILE *file = fopen(argv[1], "rb");
    if (!file)
    {
        printf("No file, cant open");
        return 0;
    }

    fread(&ch8.memory[STARTING_MEM_ADDRESS], 1, (4096 - STARTING_MEM_ADDRESS), file);
    fclose(file);

    chip8_initialization(&ch8);
    Display display;

    display_init(&display);

    uint32_t start_tick;
    uint32_t frame_speed;
    SDL_Event event;

    bool running = true;

    while (running) // main loop starts here
    {
        start_tick = SDL_GetTicks();
        cycle(&ch8);
        if (ch8.drawScreen == true)
        {
            display_draw(&display, ch8.video);
            ch8.drawScreen = false;
        }

        frame_speed = SDL_GetTicks() - start_tick;
        if (frame_speed < MS_PER_CYCLE)
        {
            SDL_Delay(MS_PER_CYCLE - frame_speed);
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
    }
    display_cleanup(&display);
    return 0;
}
