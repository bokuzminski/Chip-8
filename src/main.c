#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

#undef main

#define SCREEN_W 640
#define SCREEN_H 320
#define SCREEN_BPP 32
#define W 64
#define H 32

void chip8_initialization(Chip8 *ch8, FILE *file);
void execute_opcodes(Chip8 *ch8);
void draw_screen(Chip8 *ch8);

int main(int argc, char *argv[])
{

    SDL_Init(SDL_INIT_EVERYTHING);

    Chip8 ch8;

    FILE *game = fopen(argv[1], "rb");

    chip8_initialization(&ch8, game);
    Display display;

    if (display_init(&display))
    {
        fprintf(stderr, "Unable to initialize the display!\n");
        return -1;
    }
    uint32_t start_tick;
    uint32_t frame_speed;
    SDL_Event event;

    bool running = true;

    while (running) // main loop starts here
    {
        start_tick = SDL_GetTicks();
        //execute_opcodes(&ch8);
        if (ch8.drawScreen)
        {
            // display_draw(&display, ch8.video);
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

    return 0;
}

void draw_screen(Chip8 *ch8)
{

    return;
}

void execute_opcodes(Chip8 *ch8)
{
    int y, x, Vx, Vy, times, i;
    unsigned height, pixel;

    for (times = 0; times < 10; times++)
    {
        ch8->opcode = ch8->memory[ch8->PC] << 8 | ch8->memory[ch8->PC + 1];
        printf("Executing %04X at %04X , I:%02X SP:%02X\n", ch8->opcode, ch8->PC, ch8->index_register, ch8->SP);
        switch (ch8->opcode & 0xF000)
        {
        case 0x0000:
            switch (ch8->opcode & 0x000F)
            {
            case 0x0000:
                memset(ch8->video, 0, sizeof(ch8->video));
                ch8->PC += 2;
                break;
            case 0x000E:
                ch8->PC = ch8->stack[(--ch8->SP) & 0xF] + 2;
                break;
            }
            break;
        case 0x1000:
            ch8->PC = ch8->opcode & 0x0FFF;
            break;
        case 0x2000:
            ch8->stack[ch8->SP] = ch8->PC;
            ch8->SP++;
            ch8->PC = (ch8->opcode & 0x0FFF);
            break;
        case 0x3000:
            if (ch8->V[((ch8->opcode & 0x0F00) >> 8)] == (ch8->opcode & 0x00FFF))
            {
                ch8->PC += 4;
            }
            else
            {
                ch8->PC += 2;
            }
            break;
        case 0x6000:
            ch8->V[(ch8->opcode & 0x0F00) >> 8] = (ch8->opcode & 0x00FF);
            ch8->PC += 2;
            break;
        case 0x7000:
            ch8->V[(ch8->opcode & 0x0F00) >> 8] += (ch8->opcode & 0x00FF);
            ch8->PC += 2;
            break;
        case 0xA000:
            ch8->index_register = ch8->opcode & 0x0FFF;
            ch8->PC += 2;
            break;
        case 0xD000:
            Vx = (ch8->opcode & 0x0F00) >> 8;
            Vy = (ch8->opcode & 0x00F0) >> 4;
            height = (ch8->opcode & 0x000F); // N in the opcode
            x = ch8->V[Vx];
            y = ch8->V[Vy];
            uint8_t pixel;
            ch8->V[0xF] = 0;
            for (i = 0; i < height; i++)
            {
                pixel = ch8->memory[ch8->index_register + i];
                for (int j = 0; j < 8; j++)
                {
                    if ((pixel & (0x80 >> j)) != 0)
                    {
                        /*  if (ch8->video[y + j][x + i] == 1)
                        {
                            ch8->V[0xF] = 1;
                        } */
                        //ch8->video[y + j][x + i] ^= 1;
                    }
                }
            }
            ch8->PC += 2;
            break;
        default:
            printf("Wrong opcode: %X\n", ch8->opcode);
            break;
        }
    }
}