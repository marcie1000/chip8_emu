#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <SDL.h>

#define MEMORY_SIZE 4096
#define START_ADRESS 512U
#define NB_REGISTERS 16
#define NB_STACK 16
#define OPCODE_NB 35
#define NB_KEYS 16

typedef struct s_jump {
    Uint16 mask[OPCODE_NB];
    Uint16 id[OPCODE_NB];
} s_jump;

typedef struct s_cpu {
    Uint8 memory[MEMORY_SIZE];
    Uint16 pc; //program counter
    Uint8 V[NB_REGISTERS]; //registers
    Uint16 I; //adress register
    Uint16 jump[NB_STACK];
    Uint8 jump_nb;
    Uint8 sys_counter;
    Uint8 sound_counter;
    s_jump jump_table;
    SDL_bool keys[NB_KEYS];
} s_cpu;

#include "emulator.h"

extern void initialize_cpu(s_cpu *cpu);
extern void count(s_cpu *cpu);
extern int load_rom(s_cpu *cpu, const char path[]);
extern Uint16 get_opcode(s_cpu *cpu);
extern Uint8 get_action(s_jump *table, Uint16 opcode);
extern void initialize_jump_table(s_jump *table);
extern void init_memory_fonts(Uint8 *memory);
extern void init_opcodes_pointers(void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8));
extern void init_opcodes_pointers(void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8));
extern void interpret(s_emulator *emulator, void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8));

#endif //CHIP8_CPU_H

