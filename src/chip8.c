#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#include "fontset.h"

void chip8_initialization(Chip8 *ch8)
{
    for (int i = 0; i < 80; i++)
    {
        ch8->memory[i] = fontset[i];
    }

    memset(ch8->video, 0xFF000000, sizeof(ch8->video));
    memset(ch8->stack, 0, sizeof(ch8->stack));
    memset(ch8->V, 0, sizeof(ch8->V));
    memset(ch8->keys, 0, sizeof(ch8->keys));

    ch8->PC = 0x200;
    ch8->SP = 0;
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
    update_timers(ch8);
}

void update_timers(Chip8 *ch8)
{
    if (ch8->delay_timer > 0)
    {
        ch8->delay_timer--;
    }
    if (ch8->sound_timer > 0)
    {
        ch8->sound_timer--;
    }
    return;
}

void execute_opcodes(Chip8 *ch8)
{

    printf("Executing 0x%04X at %04X , I:%02X SP:%02X\n", ch8->opcode, ch8->PC, ch8->index_register, ch8->SP);

    switch (ch8->opcode & 0xF000)
    {
    case 0x0000:
        switch (ch8->opcode & 0x00FF)
        {
        case 0x00E0:
        {
            memset(ch8->video, 0, sizeof(ch8->video));
            ch8->drawScreen = true;
            ch8->PC += 2;
            break;
        }
        case 0x00EE:
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
    {
        uint8_t x = (ch8->opcode & 0x0F00) >> 8;
        uint8_t kk = ch8->opcode & 0x00FF;

        if (ch8->V[x] == kk)
        {
            ch8->PC += 4;
        }
        else
        {

            ch8->PC += 2;
        }
        break;
    }
    case 0x4000:
    {
        uint8_t x = (ch8->opcode & 0x0F00) >> 8;
        uint8_t kk = ch8->opcode & 0x00FF;

        if (ch8->V[x] != kk)
        {
            ch8->PC += 4;
        }
        ch8->PC += 2;
        break;
    }
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
    case 0x8000:
        switch (ch8->opcode & 0x000F)
        {
        case 0x0000:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;

            ch8->V[x] = ch8->V[y];

            break;
        }
        case 0x0001:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;

            ch8->V[x] = ch8->V[x] | ch8->V[y];

            break;
        }
        case 0x0002:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;

            ch8->V[x] = ch8->V[x] & ch8->V[y];
            break;
        }
        case 0x0003:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;

            ch8->V[x] ^= ch8->V[y];
            break;
        }
        case 0x0004:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;
            uint16_t sum = ch8->V[x] + ch8->V[y];

            if (sum > 255)
            {
                ch8->V[0xF] = 1;
            }
            else
            {
                ch8->V[0xF] = 0;
            }
            ch8->V[x] = sum & 0xFF;
            break;
        }
        case 0x0005:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;

            if (ch8->V[x] > ch8->V[y])
            {
                ch8->V[0xF] = 1;
            }
            else
            {
                ch8->V[0xF] = 0;
            }
            ch8->V[x] -= ch8->V[y];
            break;
        }
        case 0x0006:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            ch8->V[x] >>= 1;
            ch8->V[0xF] = (ch8->V[x] & 0x1);
            ch8->PC += 2;
            break;
        }
        case 0x0007:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;
            uint8_t y = (ch8->opcode & 0x00F0) >> 4;

            if (ch8->V[y] > ch8->V[x])
            {
                ch8->V[0xF] = 1;
            }
            else
            {
                ch8->V[0xF] = 0;
            }
            ch8->V[x] = ch8->V[y] - ch8->V[x];
            ch8->PC += 2;
            break;
        }
        case 0x000E:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            ch8->V[0xF] = (ch8->V[x] & 0x80) >> 7;
            ch8->V[x] <<= 1;
            break;
        }
        default:
            printf("Unknown opcode 8XY%x", ch8->opcode);
            exit(-1);
        }
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
        ch8->PC = ((ch8->opcode & 0x0FFF) + ch8->V[0x0]);
        break;
    case 0xC000:
    {
        uint8_t rand_num = rand() % 256;

        ch8->V[(ch8->opcode & 0x0F00) >> 8] = rand_num + (ch8->opcode & 0x00FF);
        ch8->PC += 2;
        break;
    }
    case 0xD000:
    {
        uint8_t Vx = (ch8->opcode & 0x0F00) >> 8;
        uint8_t Vy = (ch8->opcode & 0x00F0) >> 4;

        uint8_t n = (ch8->opcode & 0x000F);

        uint8_t xPos = ch8->V[Vx];
        uint8_t yPos = ch8->V[Vy];

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
    case 0xE000:
    {
        switch (ch8->opcode & 0x00FF)
        {
        case 0x009E:
            if (ch8->keys[ch8->V[(ch8->opcode & 0x0F00) >> 8]] != 0)
            {
                ch8->PC += 4;
            }
            else
            {
                ch8->PC += 2;
            }
            break;
        case 0x00A1:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            if (!ch8->keys[ch8->V[x]])
            {
                ch8->PC += 4;
            }
            ch8->PC += 2;
            break;
        }
        }
        break;
    }
    break;
    case 0xF000:
        switch (ch8->opcode & 0x00FF)
        {
        case 0x0007:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            ch8->V[x] = ch8->delay_timer;
            ch8->PC += 2;
            break;
        }
        case 0x000A:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            if (ch8->keys[0])
            {
                ch8->V[x] = 0;
                ch8->PC += 2;
            }
            else if (ch8->keys[1])
            {
                ch8->V[x] = 1;
            }
            else if (ch8->keys[2])
            {
                ch8->V[x] = 2;
            }
            else if (ch8->keys[3])
            {
                ch8->V[x] = 3;
            }
            else if (ch8->keys[4])
            {
                ch8->V[x] = 4;
            }
            else if (ch8->keys[5])
            {
                ch8->V[x] = 5;
            }
            else if (ch8->keys[6])
            {
                ch8->V[x] = 6;
            }
            else if (ch8->keys[7])
            {
                ch8->V[x] = 7;
            }
            else if (ch8->keys[8])
            {
                ch8->V[x] = 8;
            }
            else if (ch8->keys[9])
            {
                ch8->V[x] = 9;
            }
            else if (ch8->keys[10])
            {
                ch8->V[x] = 10;
            }
            else if (ch8->keys[11])
            {
                ch8->V[x] = 11;
            }
            else if (ch8->keys[12])
            {
                ch8->V[x] = 12;
            }
            else if (ch8->keys[13])
            {
                ch8->V[x] = 13;
            }
            else if (ch8->keys[14])
            {
                ch8->V[x] = 14;
            }
            else if (ch8->keys[15])
            {
                ch8->V[x] = 15;
            }
            break;
        }
        case 0x0015:
            ch8->delay_timer = ch8->V[(ch8->opcode & 0x0F00) >> 8];
            ch8->PC += 2;
            break;
        case 0x0018:
            ch8->sound_timer = ch8->V[(ch8->opcode & 0x0F00) >> 8];
            ch8->PC += 2;
            break;
        case 0x001E:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            ch8->index_register += ch8->V[x];
            ch8->PC += 2;
            break;
        }
        case 0x0029:
        {
            uint8_t x = (ch8->opcode & 0x0F00) >> 8;

            ch8->index_register = ch8->V[x] * 5;
            ch8->PC += 2;
            break;
        }
        case 0x0033:
            ch8->memory[ch8->index_register] = ch8->V[(ch8->opcode & 0x0F00) >> 8] / 100;
            ch8->memory[ch8->index_register + 1] = (ch8->V[(ch8->opcode & 0x0F00) >> 8] / 10) % 10;
            ch8->memory[ch8->index_register + 2] = (ch8->V[(ch8->opcode & 0x0F00) >> 8] % 100) % 10;
            ch8->PC += 2;
            break;
        case 0x0055:
        {
            uint8_t end_v_reg = (ch8->opcode & 0x0F00) >> 8;

            for (size_t i = 0; i < end_v_reg; i++)
            {
                ch8->memory[ch8->index_register + i] = ch8->V[i];
            }
            // ch8->index_register += (end_v_reg + 1);
            ch8->PC += 2;
            break;
        }
        case 0x0065:
            for (int l = 0; l < ((ch8->opcode & 0x0F00) >> 8); l++)
            {
                ch8->V[l] = ch8->memory[ch8->index_register + l];
            }
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