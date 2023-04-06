#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "chip8_cpu.h"
#include "emulator.h"
#include "opcodes.h"

void initialize_cpu(s_cpu *cpu)
{
    memset(cpu, 0, sizeof(*cpu));
    initialize_jump_table(&cpu->jump_table);
    cpu->pc = START_ADRESS;
}

void count(s_cpu *cpu)
{
    if(cpu->sys_counter > 0)
        cpu->sys_counter--;
    if(cpu->sound_counter > 0)
        cpu->sound_counter--;
}

int load_rom(s_cpu *cpu, const char path[])
{
    FILE *rom = fopen(path, "rb");
    if(NULL == rom)
    {
        perror("Error fopen:");
        return EXIT_FAILURE;
    }
    
    if(MEMORY_SIZE < fread(&cpu->memory[START_ADRESS], sizeof(cpu->memory[0]), MEMORY_SIZE - START_ADRESS, rom))
    {
        fprintf(stderr, "Can't load rom: size can't exceed %d bytes.\n", MEMORY_SIZE - START_ADRESS);
        return EXIT_FAILURE;
    }
    
    fclose(rom);
    
    return EXIT_SUCCESS;
}

Uint16 get_opcode(s_cpu *cpu)
{
    return (cpu->memory[cpu->pc] << 8) + cpu->memory[cpu->pc + 1];
}

Uint8 get_action(s_jump *table, Uint16 opcode)
{
    for(size_t i = 0; i < OPCODE_NB; i++)
    {
        if((table->mask[i] & opcode) == table->id[i])
            return i;
    }
    fprintf(stderr, "Bad action, unknown opcode %d.\n", opcode);
    return 0;
}

void initialize_jump_table(s_jump *table)
{
    table->mask[0] = 0x0000; table->id[0] = 0x0FFF;         // 0NNN
    table->mask[1] = 0xFFFF; table->id[1] = 0x00E0;         // 00E0
    table->mask[2] = 0xFFFF; table->id[2] = 0x00EE;         // 00EE
    table->mask[3] = 0xF000; table->id[3] = 0x1000;         // 1NNN
    table->mask[4] = 0xF000; table->id[4] = 0x2000;         // 2NNN
    table->mask[5] = 0xF000; table->id[5] = 0x3000;         // 3XNN
    table->mask[6] = 0xF000; table->id[6] = 0x4000;         // 4XNN
    table->mask[7] = 0xF00F; table->id[7] = 0x5000;         // 5XY0
    table->mask[8] = 0xF000; table->id[8] = 0x6000;         // 6XNN
    table->mask[9] = 0xF000; table->id[9] = 0x7000;         // 7XNN
    table->mask[10] = 0xF00F; table->id[10] = 0x8000;         // 8XY0
    table->mask[11] = 0xF00F; table->id[11] = 0x8001;         // 8XY1
    table->mask[12] = 0xF00F; table->id[12] = 0x8002;         // 8XY2
    table->mask[13] = 0xF00F; table->id[13] = 0x8003;         // 8XY3
    table->mask[14] = 0xF00F; table->id[14] = 0x8004;         // 8XY4
    table->mask[15] = 0xF00F; table->id[15] = 0x8005;         // 8XY5
    table->mask[16] = 0xF00F; table->id[16] = 0x8006;         // 8XY6
    table->mask[17] = 0xF00F; table->id[17] = 0x8007;         // 8XY7
    table->mask[18] = 0xF00F; table->id[18] = 0x800E;         // 8XYE
    table->mask[19] = 0xF00F; table->id[19] = 0x9000;         // 9XY0
    table->mask[20] = 0xF000; table->id[20] = 0xA000;         // ANNN
    table->mask[21] = 0xF000; table->id[21] = 0xB000;         // BNNN
    table->mask[22] = 0xF000; table->id[22] = 0xC000;         // CXNN
    table->mask[23] = 0xF000; table->id[23] = 0xD000;         // DXYN
    table->mask[24] = 0xF0FF; table->id[24] = 0xE09E;         // EX9E
    table->mask[25] = 0xF0FF; table->id[25] = 0xE0A1;         // EXA1
    table->mask[26] = 0xF0FF; table->id[26] = 0xF007;         // FX07
    table->mask[27] = 0xF0FF; table->id[27] = 0xF00A;         // FX0A
    table->mask[28] = 0xF0FF; table->id[28] = 0xF015;         // FX15
    table->mask[29] = 0xF0FF; table->id[29] = 0xF018;         // FX18
    table->mask[30] = 0xF0FF; table->id[30] = 0xF01E;         // FX1E
    table->mask[31] = 0xF0FF; table->id[31] = 0xF029;         // FX29
    table->mask[32] = 0xF0FF; table->id[32] = 0xF033;         // FX33
    table->mask[33] = 0xF0FF; table->id[33] = 0xF055;         // FX55
    table->mask[34] = 0xF0FF; table->id[34] = 0xF065;         // FX65
}

void interpret(s_emulator *emulator, void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8))
{
    Uint16 opcode = get_opcode(&emulator->cpu);
    Uint8 b3,b2,b1;
    b3 = (opcode & (0x0F00U)) >> 8;
    b2 = (opcode & (0x00F0U)) >> 4;
    b1 = (opcode & (0x000FU));
    Uint8 action = get_action(&emulator->cpu.jump_table, opcode);
    if(action < OPCODE_NB)
        (*opcode_functions[action])(emulator, b3, b2, b1);
//    printf("%d\n", action);
    emulator->cpu.pc += 2;
}

void init_memory_fonts(Uint8 *memory)
{
    memory[0] = 0xF0;  memory[1] = 0x90;  memory[2] = 0x90;  memory[3] = 0x90;  memory[4] = 0xF0;  //0
    memory[5] = 0x20;  memory[6] = 0x60;  memory[7] = 0x20;  memory[8] = 0x20;  memory[9] = 0x70;  //1
    memory[10] = 0xF0; memory[11] = 0x10; memory[12] = 0xF0; memory[13] = 0x80; memory[14] = 0xF0; //2
    memory[15] = 0xf0; memory[16] = 0x10; memory[17] = 0xf0; memory[18] = 0x10; memory[19] = 0xf0; //3
    memory[20] = 0x90; memory[21] = 0x90; memory[22] = 0xf0; memory[23] = 0x10; memory[24] = 0x10; //4
    memory[25] = 0xf0; memory[26] = 0x80; memory[27] = 0xf0; memory[28] = 0x10; memory[29] = 0xf0; //5
    memory[30] = 0xf0; memory[31] = 0x80; memory[32] = 0xf0; memory[33] = 0x90; memory[34] = 0xf0; //6
    memory[35] = 0xf0; memory[36] = 0x10; memory[37] = 0x20; memory[38] = 0x40; memory[39] = 0x40; //7
    memory[40] = 0xf0; memory[41] = 0x90; memory[42] = 0xf0; memory[43] = 0x90; memory[44] = 0xf0; //8
    memory[45] = 0xf0; memory[46] = 0x90; memory[47] = 0xf0; memory[48] = 0x10; memory[49] = 0xf0; //9
    memory[50] = 0xf0; memory[51] = 0x90; memory[52] = 0xf0; memory[53] = 0x90; memory[54] = 0x90; //A
    memory[55] = 0xE0; memory[56] = 0x90; memory[57] = 0xf0; memory[58] = 0x90; memory[59] = 0xE0; //B
    memory[60] = 0xf0; memory[61] = 0x80; memory[62] = 0x80; memory[63] = 0x80; memory[64] = 0xf0; //C
    memory[65] = 0xE0; memory[66] = 0x90; memory[67] = 0x90; memory[68] = 0x90; memory[69] = 0xE0; //D
    memory[70] = 0xf0; memory[71] = 0x80; memory[72] = 0xf0; memory[73] = 0x80; memory[74] = 0xf0; //E
    memory[75] = 0xf0; memory[76] = 0x80; memory[77] = 0xf0; memory[78] = 0x80; memory[79] = 0x80; //F
}

void init_opcodes_pointers(void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8))
{
    opcode_functions[0] = &opcode_0NNN;
    opcode_functions[1] = &opcode_00E0;
    opcode_functions[2] = &opcode_00EE;
    opcode_functions[3] = &opcode_1NNN;
    opcode_functions[4] = &opcode_2NNN;
    opcode_functions[5] = &opcode_3XNN;
    opcode_functions[6] = &opcode_4XNN;
    opcode_functions[7] = &opcode_5XY0;
    opcode_functions[8] = &opcode_6XNN;
    opcode_functions[9] = &opcode_7XNN;
    opcode_functions[10] = &opcode_8XY0;
    opcode_functions[11] = &opcode_8XY1;
    opcode_functions[12] = &opcode_8XY2;
    opcode_functions[13] = &opcode_8XY3;
    opcode_functions[14] = &opcode_8XY4;
    opcode_functions[15] = &opcode_8XY5;
    opcode_functions[16] = &opcode_8XY6;
    opcode_functions[17] = &opcode_8XY7;
    opcode_functions[18] = &opcode_8XYE;
    opcode_functions[19] = &opcode_9XY0;
    opcode_functions[20] = &opcode_ANNN;
    opcode_functions[21] = &opcode_BNNN;
    opcode_functions[22] = &opcode_CXNN;
    opcode_functions[23] = &opcode_DXYN;
    opcode_functions[24] = &opcode_EX9E;
    opcode_functions[25] = &opcode_EXA1;
    opcode_functions[26] = &opcode_FX07;
    opcode_functions[27] = &opcode_FX0A;
    opcode_functions[28] = &opcode_FX15;
    opcode_functions[29] = &opcode_FX18;
    opcode_functions[30] = &opcode_FX1E;
    opcode_functions[31] = &opcode_FX29;
    opcode_functions[32] = &opcode_FX33;
    opcode_functions[33] = &opcode_FX55;
    opcode_functions[34] = &opcode_FX65;    
}

