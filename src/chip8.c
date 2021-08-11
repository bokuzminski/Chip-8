#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#include "fontset.h"

void chip8_initialization(Chip8 *ch8)
{
    for (size_t i = 0; i < 80; i++)
    {
        ch8->memory[0x50 + i] = fontset[i];
    }

    memset(ch8->video, 0xFF000000, sizeof(ch8->video));
    memset(ch8->stack, 0, sizeof(ch8->stack));
    memset(ch8->V, 0, sizeof(ch8->V));

    ch8->PC = 0x200;
    ch8->SP &= 0;
    ch8->opcode = 0;
    ch8->index_register = 0;
    ch8->sound_timer = 0;
    ch8->delay_timer = 0;
    ch8->drawScreen = false;

    srand(time(NULL));
}

void fetch_opcodes(Chip8 *ch8)
{
    ch8->opcode = ch8->memory[ch8->PC] << 8 | ch8->memory[ch8->PC + 1];
}

void cycle(Chip8 *ch8)
{
    fetch_opcodes(ch8);
    execute_opcodes(ch8);
}

void execute_opcodes(Chip8 *ch8)
{
    uint8_t Vx, Vy, xPos, yPos, end_v_reg;

    printf("Executing 0x%04X at %04X , I:%02X SP:%02X\n", ch8->opcode, ch8->PC, ch8->index_register, ch8->SP);

    switch (ch8->opcode & 0xF000)
    {
    case 0x0000:
        switch (ch8->opcode & 0x000F)
        {
        case 0x0000:
            memset(ch8->video, 0, sizeof(ch8->video));
            ch8->drawScreen = true;
            ch8->PC += 2;
            break;
        case 0x000E:
        {
            ch8->SP--;
            ch8->PC = ch8->stack[ch8->SP];
            ch8->PC += 2;
            break;
        }
        }
        break;
    case 0x1000:
    {
        uint16_t nnn = ch8->opcode & 0x0FFF;
        ch8->PC = nnn;
        break;
    }
    case 0x2000:
        ch8->stack[ch8->SP] = ch8->PC;
        ch8->SP++;
        ch8->PC = (ch8->opcode & 0x0FFF);
        break;
    case 0x3000:
        if (ch8->V[((ch8->opcode & 0x0F00) >> 8)] == (ch8->opcode & 0x00FF))
        {
            ch8->PC += 4;
        }
        else
        {
            ch8->PC += 2;
        }
        break;
    case 0x4000:
        if (ch8->V[((ch8->opcode & 0x0F00) >> 8)] != (ch8->opcode & 0x00FF))
        {
            ch8->PC += 4;
        }
        else
        {
            ch8->PC += 2;
        }
        break;
    case 0x5000:
        if (ch8->V[(ch8->opcode & 0x0F00) >> 8] == ch8->V[(ch8->opcode & 0x00F0) >> 4])
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
    case 0x9000:
        if (ch8->V[(ch8->opcode & 0x0F00) >> 8] != ch8->V[(ch8->opcode & 0x00F0) >> 4])
        {
            ch8->PC += 4;
        }
        else
        {

            ch8->PC += 2;
        }
        break;
    case 0xA000:
        ch8->index_register = ch8->opcode & 0x0FFF;
        ch8->PC += 2;
        break;
    case 0xB000:
        ch8->PC = ((ch8->opcode & 0x0FFF) + ch8->V[0]);
        break;
    case 0xC000:
        ch8->V[(ch8->opcode & 0x0F00) >> 8] = rand() + (ch8->opcode & 0x00FF);
        ch8->PC += 2;
        break;
    case 0xD000:
    {
        Vx = (ch8->opcode & 0x0F00) >> 8;
        Vy = (ch8->opcode & 0x00F0) >> 4;

        uint8_t n = (ch8->opcode & 0x000F);

        xPos = ch8->V[Vx];
        yPos = ch8->V[Vy];

        ch8->V[0xF] = 0;

        for (unsigned int row = 0; row < n; ++row)
        {
            uint8_t spriteByte = ch8->memory[ch8->index_register + row];

            for (unsigned int col = 0; col < 8; ++col)
            {
                uint8_t spritePixel = spriteByte & (0x80 >> col);
                uint32_t *screenPixel = &ch8->video[xPos + col + ((yPos + row) * 64)];

                if (spritePixel)
                {
                    if (*screenPixel == 0xFFFFFFFF)
                    {
                        ch8->V[0xF] = 1;
                    }
                    *screenPixel ^= 0xFFFFFFFF;
                }
            }
        }
        ch8->drawScreen = true;
        ch8->PC += 2;
    }
    break;
    case 0xF000:
        switch (ch8->opcode & 0x00FF)
        {
        case 0x0007:
            ch8->V[(ch8->opcode & 0x0F00) >> 8] = ch8->delay_timer;
            ch8->PC += 2;
            break;
        case 0x000A:
            //keyboard related
            break;
        case 0x0015:
            ch8->delay_timer = ch8->V[(ch8->opcode & 0x0F00) >> 8];
            ch8->PC += 2;
            break;
        case 0x0018:
            ch8->sound_timer = ch8->V[(ch8->opcode & 0x0F00) >> 8];
            ch8->PC += 2;
            break;
        case 0x001E:
            ch8->index_register = ch8->index_register + ch8->V[(ch8->opcode & 0x0F00) >> 8];
            ch8->PC += 2;
            break;
        case 0x0029:
            ch8->index_register = (ch8->V[(ch8->opcode & 0x0F00) >> 8] * 0x5);
            ch8->PC += 2;
            break;
        case 0x0033:
            ch8->memory[ch8->index_register] = ch8->V[(ch8->opcode & 0x0F00) >> 8] / 100;
            ch8->memory[ch8->index_register + 1] = (ch8->V[(ch8->opcode & 0x0F00) >> 8] / 10) % 10;
            ch8->memory[ch8->index_register + 2] = (ch8->V[(ch8->opcode & 0x0F00) >> 8] % 100) % 10;
            ch8->PC += 2;
            break;
        case 0x0055:
            end_v_reg = (ch8->opcode & 0x0F00) >> 8;
            for (size_t i = 0; i < end_v_reg; i++)
            {
                ch8->memory[ch8->index_register + i] = ch8->V[i];
            }
            ch8->index_register += (end_v_reg + 1);
            ch8->PC += 2;

            break;
        default:
            printf("ERROR: Unrecognized opcode 0x%X\n", ch8->opcode);
        }
        break;
    default:
        printf("Wrong opcode: 0x%X\n", ch8->opcode);
    }
}