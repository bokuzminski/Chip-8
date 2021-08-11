#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

#undef main

#define STARTING_MEM_ADDRESS 0x200
#define SCREEN_W 640
#define SCREEN_H 320
#define SCREEN_BPP 32
#define W 64
#define H 32

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

    fread(&ch8.memory[STARTING_MEM_ADDRESS], 1, (4096 - 0x200), file);
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
        if (frame_speed < (1000 / 60))
        {
            SDL_Delay((1000 / 60) - frame_speed);
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }
    display_cleanup(&display);
    return 0;
}
