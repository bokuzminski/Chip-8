#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
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
    u_int32_t video[64 * 32];
    u_int16_t opcode;
    uint8_t keys[16];

    bool drawScreen;
} Chip8;

const static uint8_t KEYMAP[16] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};

void chip8_initialization(Chip8 *ch8);
void fetch_opcodes(Chip8 *ch8);
void cycle(Chip8 *ch8);
void update_timers(Chip8 *ch8);
void execute_opcodes(Chip8 *ch8);