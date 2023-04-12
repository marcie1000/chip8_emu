#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "chip8_cpu.h"
#include "chip8_graphics.h"
#include "emulator.h"
#include "opcodes.h"

void opcode_unimplemented(s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1)
{
    fprintf(stderr, "%x instruction unimplemented!\n", 
            (emulator->cpu.memory[emulator->cpu.pc] << 8) + 
            (emulator->cpu.memory[emulator->cpu.pc + 1]));
}

void opcode_0NNN(UNUSED s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1)
//does nothing
{
    return;
}

void opcode_00E0(s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1)
//clears the screen
{
    clear_screen(&emulator->screen);
}

void opcode_00EE(s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1)
//returns from a subroutine
{
    s_cpu *cpu = &emulator->cpu;
    if(cpu->jump_nb > 0)
    {
        cpu->jump_nb--;
        cpu->pc = cpu->jump[cpu->jump_nb];
    }
}

void opcode_1NNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N)
//jumps to adress NNN
{
    emulator->cpu.pc = (b3_N << 8) + (b2_N << 4) + b1_N - 2;
}

void opcode_2NNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N)
//Calls subroutine at NNN
{
    s_cpu *cpu = &emulator->cpu;
    cpu->jump[cpu->jump_nb] = cpu->pc;
    if(cpu->jump_nb < (NB_STACK - 1))
        cpu->jump_nb++;
        
    cpu->pc = (b3_N << 8) + (b2_N << 4) + b1_N - 2;
}

void opcode_3XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N)
//Skips the next instruction if VX equals NN (usually the next instruction 
//is a jump to skip a code block). 
{
    s_cpu *cpu = &emulator->cpu;
    if(cpu->V[b3_X] == ((b2_N << 4) + b1_N))
        cpu->pc += 2;
}

void opcode_4XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N)
//Skips the next instruction if VX does not equal NN 
//(usually the next instruction is a jump to skip a code block). 
{
    s_cpu *cpu = &emulator->cpu;
    if(cpu->V[b3_X] != ((b2_N << 4) + b1_N))
        cpu->pc += 2;
}

void opcode_5XY0(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_0)
//Skips the next instruction if VX equals VY 
//(usually the next instruction is a jump to skip a code block). 
{
    s_cpu *cpu = &emulator->cpu;
    if(cpu->V[b3_X] == cpu->V[b2_Y])
        cpu->pc += 2;
}

void opcode_6XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N)
//Sets VX to NN. 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] = ((b2_N << 4) + b1_N);
}

void opcode_7XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N)
//Adds NN to VX (carry flag is not changed). 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] += ((b2_N << 4) + b1_N);
}

void opcode_8XY0(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_0)
//Sets VX to the value of VY. 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] = cpu->V[b2_Y];
}

void opcode_8XY1(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_1)
//Sets VX to VX or VY. (bitwise OR operation) 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] |= cpu->V[b2_Y];
}

void opcode_8XY2(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_2)
//Sets VX to VX and VY. (bitwise AND operation) 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] &= cpu->V[b2_Y];
}

void opcode_8XY3(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_3)
//Sets VX to VX xor VY. 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] ^= cpu->V[b2_Y];
}

void opcode_8XY4(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_4)
//Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not. 
{
    s_cpu *cpu = &emulator->cpu;
    Uint16 VX_tmp = cpu->V[b3_X];
    Uint16 VY_tmp = cpu->V[b2_Y];
    VX_tmp += VY_tmp;
    cpu->V[0xF] = VX_tmp > 255;
    VX_tmp &= ~(0xFF00U);
    cpu->V[b3_X] = (Uint8)VX_tmp;    
}

void opcode_8XY5(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_5)
//VY is subtracted from VX. VF is set to 0 when there's a borrow, 
//and 1 when there is not. 
{
    s_cpu *cpu = &emulator->cpu;
    Sint16 VX_tmp = cpu->V[b3_X];
    Sint16 VY_tmp = cpu->V[b2_Y];
    cpu->V[0xF] = 1;
    VX_tmp -= VY_tmp;
    cpu->V[0xF] = (VX_tmp > 0);
    VX_tmp &= ~(0xFF00U);
    cpu->V[b3_X] = (Uint8)VX_tmp;
}

void opcode_8XY6(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_Y, UNUSED Uint8 b1_6)
//Stores the least significant bit of VX in VF and then 
//shifts VX to the right by 1.
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[0xF] = (0x01U & cpu->V[b3_X]) > 0;
    cpu->V[b3_X] >>= 1;
}

void opcode_8XY7(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_7)
//Sets VX to VY minus VX. VF is set to 0 when there's a borrow, 
//and 1 when there is not. 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] = cpu->V[b2_Y] - cpu->V[b3_X];
    //VF = VY < VX
    cpu->V[0xF] = cpu->V[b3_X] <= cpu->V[b2_Y];
}

void opcode_8XYE(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_Y, UNUSED Uint8 b1_E)
//Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[0xF] = (0x80U & cpu->V[b3_X]) > 0; //0b10000000
    cpu->V[b3_X] <<= 1;
}

void opcode_9XY0(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_0)
//Skips the next instruction if VX does not equal VY. 
//(Usually the next instruction is a jump to skip a code block); 
{
    s_cpu *cpu = &emulator->cpu;
    if(cpu->V[b3_X] != cpu->V[b2_Y])
        cpu->pc += 2;
}

void opcode_ANNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N)
//Sets I to the address NNN. 
{
    emulator->cpu.I = (b3_N << 8) + (b2_N << 4) + b1_N;
}

void opcode_BNNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N)
//Jumps to the address NNN plus V0. 
{
    Uint32 tmp = (b3_N << 8) + (b2_N << 4) + b1_N - 2 + emulator->cpu.V[0];
    if(tmp >= MEMORY_SIZE)
    {
        fprintf(stderr, "Memory overflow %x.\n", tmp);
        return;
    }
    emulator->cpu.pc = (Uint16)tmp;
}

double random_double(void)
{
    return (double)rand() / (double)RAND_MAX;
}

int rand_int(int min, int max)
{
    return random_double() * (max - min) + min;
}

void opcode_CXNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N)
//Sets VX to the result of a bitwise and operation on a random number 
//(Typically: 0 to 255) and NN. 
{
    emulator->cpu.V[b3_X] = rand_int(0, 0xFF) & ((b2_N << 4) + b1_N);
}

void opcode_DXYN(s_emulator *emulator, Uint8 b3_VX, Uint8 b2_VY, Uint8 b1_VN)
//draws....
{
    s_cpu *cpu = &emulator->cpu;
    s_screen *screen = &emulator->screen;
    cpu->V[0xF] = 0;
    
    for(size_t i = 0; i < b1_VN; i++)
    {
        Uint8 binary = cpu->memory[cpu->I + i];
        Uint8 y = cpu->V[b2_VY] + i;
        for(size_t j = 0; j < 8; j++)
        {
            Uint8 x = cpu->V[b3_VX] + j;
            //if bit is 1 and coordinates are not outside of screen
            if(y < PIXEL_BY_HEIGHT && x < PIXEL_BY_WIDTH && ((binary << j) & 0x80)) //0b10000000
            {
                if(screen->pixels[x][y] == WHITE)
                    cpu->V[0xF] = 1;
                screen->pixels[x][y] = !screen->pixels[x][y]; //change la couleur du bit
            } 
        }
    }
}

void opcode_EX9E(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_9, UNUSED Uint8 b1_E)
//Skips the next instruction if the key stored in VX is pressed 
//(usually the next instruction is a jump to skip a code block). 
{
    s_cpu *cpu = &emulator->cpu;
    if(cpu->keys[cpu->V[b3_X]])
        cpu->pc += 2;
}

void opcode_EXA1(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_A, UNUSED Uint8 b1_1)
//Skips the next instruction if the key stored in VX is not pressed 
//(usually the next instruction is a jump to skip a code block). 
{
    s_cpu *cpu = &emulator->cpu;
    if(!cpu->keys[cpu->V[b3_X]])
        cpu->pc += 2;
}

void opcode_FX07(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_0, UNUSED Uint8 b1_7)
//Sets VX to the value of the delay timer. 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->V[b3_X] = cpu->sys_counter;
}

void opcode_FX33(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_3, UNUSED Uint8 b1_3)
//Stores the binary-coded decimal representation of VX, 
//with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2. 
{
    s_cpu *cpu = &emulator->cpu;
    Uint8 nb = cpu->V[b3_X];
    cpu->memory[cpu->I] = nb / 100;
    cpu->memory[cpu->I+1] = (nb / 10) % 10;
    cpu->memory[cpu->I+2] = nb % 10;
}

void opcode_FX29(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_2, UNUSED Uint8 b1_9)
//Sets I to the location of the sprite for the character in VX. 
//Characters 0-F (in hexadecimal) are represented by a 4x5 font. 
{
    s_cpu *cpu = &emulator->cpu;
    cpu->I = cpu->V[b3_X] * 5;
}

void opcode_FX0A(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_0, UNUSED Uint8 b1_A)
//A key press is awaited, and then stored in VX (blocking operation, 
//all instruction halted until next key event).
{
    s_cpu *cpu = &emulator->cpu;
    s_input *in = &emulator->in;
    
    while(!in->quit)
    {
        update_event(&emulator->in);
        manage_input(emulator);
        if(emulator->in.resize)
        {
            resize_screen(&emulator->screen);
            emulator->in.resize = SDL_FALSE;
        }
        for(size_t i = 0; i < NB_KEYS; i++)
        {
            if(cpu->keys[i])
            {
                cpu->V[b3_X] = (Uint8)i;
                return;
            }
        }
        update_screen(&emulator->screen);
        SDL_Delay(FPS_DELAY);
    }
}

void opcode_FX15(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_1, UNUSED Uint8 b1_5)
//Sets the delay timer to VX. 
{
    emulator->cpu.sys_counter = emulator->cpu.V[b3_X];
}

void opcode_FX18(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_1, UNUSED Uint8 b1_8)
//Sets the sound timer to VX. 
{
    emulator->cpu.sound_counter = emulator->cpu.V[b3_X];
}

void opcode_FX1E(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_1, UNUSED Uint8 b1_E)
//Adds VX to I. VF is not affected.
{
    emulator->cpu.I += emulator->cpu.V[b3_X];
}

void opcode_FX55(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_5, UNUSED Uint8 b1_5)
//Stores from V0 to VX (including VX) in memory, starting at address I. 
//The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d]
//In the original CHIP-8 implementation, and also in CHIP-48, 
//I is left incremented after this instruction had been executed. In SCHIP, I is left unmodified.
{
    s_cpu *cpu = &emulator->cpu;
    for(size_t j = 0; j <= b3_X; j++)
    {
        cpu->memory[cpu->I + j] = cpu->V[j];
        //cpu->I = (Uint16)j;
    }
}

void opcode_FX65(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_6, UNUSED Uint8 b1_5)
//Fills from V0 to VX (including VX) with values from memory, starting at address I. 
//The offset from I is increased by 1 for each value read, but I itself is left unmodified.[d]
//In the original CHIP-8 implementation, and also in CHIP-48, 
//I is left incremented after this instruction had been executed. In SCHIP, I is left unmodified.
{
    s_cpu *cpu = &emulator->cpu;
    for(size_t j = 0; j <= b3_X; j++)
    {
        cpu->V[j] = cpu->memory[cpu->I + j];
        //cpu->I = (Uint16)j;
    }
}
