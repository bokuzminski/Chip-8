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
void execute_opcodes(Chip8 *ch8);

void DisassembleChip8Op(uint8_t *codebuffer, int pc)
{
    uint8_t *code = &codebuffer[pc];
    uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);
    switch (firstnib)
    {
    case 0x00:
        printf("0 not handled yet");
        break;
    case 0x01:
        printf("1 not handled yet");
        break;
    case 0x02:
        printf("2 not handled yet");
        break;
    case 0x03:
        printf("3 not handled yet");
        break;
    case 0x04:
        printf("4 not handled yet");
        break;
    case 0x05:
        printf("5 not handled yet");
        break;
    case 0x06:
    {
        uint8_t reg = code[0] & 0x0f;
        printf("%-10s V%01X,#$%02x", "MVI", reg, code[1]);
    }
    break;
    case 0x07:
        printf("7 not handled yet");
        break;
    case 0x08:
        printf("8 not handled yet");
        break;
    case 0x09:
        printf("9 not handled yet");
        break;
    case 0x0a:
    {
        uint8_t addresshi = code[0] & 0x0f;
        printf("%-10s I,#$%01x%02x", "MVI", addresshi, code[1]);
    }
    break;
    case 0x0b:
        printf("b not handled yet");
        break;
    case 0x0c:
        printf("c not handled yet");
        break;
    case 0x0d:
        printf("d not handled yet");
        break;
    case 0x0e:
        printf("e not handled yet");
        break;
    case 0x0f:
        printf("f not handled yet");
        break;
    }
}

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
    fseek(game, 0L, SEEK_END);
    int fsize = ftell(game);
    fseek(game, 0L, SEEK_SET);

    chip8_initialization(&ch8, game);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(1);
            }
        }
        execute_opcodes(&ch8);
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

void execute_opcodes(Chip8 *ch8)
{
    int y, x, vx, vy, times, i;
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
            vx = ch8->V[(ch8->opcode & 0x0F00) >> 8];
            vy = ch8->V[(ch8->opcode & 0x00F0) >> 4];
            height = ch8->opcode & 0x000F;
            ch8->V[0xF] &= 0;
            for (y = 0; i < height; y++)
            {
                pixel = ch8->memory[ch8->index_register + y];
                for (x = 0; x < 8; x++)
                {
                    if (pixel & (0x80 >> x))
                    {
                        if (ch8->video[x + vx + (y + vy) * 64])
                        {
                            ch8->V[0xF] = 1;
                        }
                        ch8->video[x + vx + (y + vy) * 64] ^= 1;
                    }
                }
            }
            ch8->PC += 2;
            break;
        default:
            printf("Instruction  %04X at %04X , I:%02X SP:%02X skipped\n", ch8->opcode, ch8->PC, ch8->index_register, ch8->SP);
            break;
        }
    }
}