#include <stdio.h>

typedef struct Chip8
{
    u_int8_t memory[4096];
    u_int8_t V[16]; // registers  from V0 to VF
    u_int16_t index_register;
    u_int16_t PC;
    u_int16_t stack[16];
    u_int8_t SP;
    u_int8_t delay_timer;
    u_int8_t sound_timer;
    u_int32_t video[64][32];
    u_int16_t opcode;
} Chip8;