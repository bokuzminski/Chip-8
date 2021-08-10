#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#include "fontset.h"

void chip8_initialization(Chip8 *ch8)
{
    //  memcpy(ch8->memory, fontset, sizeof(fontset));
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