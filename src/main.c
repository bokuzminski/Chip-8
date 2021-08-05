#include <stdio.h>
#include <SDL/SDL.h>
#include "fontset.h"
#include "chip8.h"

#undef main

#define SCREEN_W 640
#define SCREEN_H 320
#define SCREEN_BPP 32
#define W 64
#define H 32

void chip8_initialization(Chip8 *ch8, FILE *file);

int main(int argc, char *argv[])
{
    SDL_Event event;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

    Chip8 ch8;

    FILE *game = fopen(argv[1], "rb");
    if (game == NULL)
    {
        printf("Can't open the ROM");
    }

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(1);
            }
        }
        chip8_initialization(&ch8, game);
    }

    return 0;
}

void chip8_initialization(Chip8 *ch8, FILE *file)
{
    int i;

    fread(ch8->memory + 0x200, 1, 4096 - 0x200, file);
    for (i = 0; i < FONTSET_SIZE; i++)
    {
        ch8->memory[i] = fontset[i];
    }
    memset(ch8->video, 0, sizeof(ch8->video));
    memset(ch8->stack, 0, sizeof(ch8->stack));
    memset(ch8->V, 0, sizeof(ch8->V));

    ch8->PC = 0x200;
    ch8->SP &= 0;
    ch8->opcode = 0x200;
}