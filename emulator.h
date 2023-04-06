#ifndef EMULATOR_H
#define EMULATOR_H

#include <SDL.h>
#include <SDL_mixer.h>

#include "chip8_cpu.h"
#include "chip8_graphics.h"

#define FPS_DELAY 16

typedef struct s_input{
    SDL_bool key[SDL_NUM_SCANCODES];
    SDL_bool quit;
    int x, y, xrel, yrel;
    int xwheel, ywheel;
    SDL_bool mouse[6];
    SDL_bool resize;
} s_input;

typedef struct s_emulator{
    s_cpu cpu;
    s_screen screen;
    s_input in;
    Mix_Chunk *bip;
    int key_table[NB_KEYS];
} s_emulator;

extern void update_event(s_input *input);
extern int initialize_SDL(s_emulator *emulator);
extern void destroy_SDL(s_emulator *emulator);
extern int initialize_emulator(s_emulator *emulator, void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8));
extern void destroy_emulator(s_emulator *emulator);
extern void emulate(s_emulator *emulator, void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8));
extern void play_sound(s_emulator *emulator);
extern void init_key_table(int *table);
extern void manage_input(s_emulator *emulator);

#endif //EMULATOR_H
