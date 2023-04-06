#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "chip8_cpu.h"
#include "chip8_graphics.h"
#include "emulator.h"

int main(int argc, char *argv[])
{
    char rom_filename[255];
    if(argc <= 1)
    {
        printf("Please give a ROM file in argument.\n");
        snprintf(rom_filename, 255, "BC_test.ch8");
    }
    else
        snprintf(rom_filename, 255, "%s", argv[1]);
    
    //rand seed
    time_t t;
    if(time(&t) == (time_t)-1)
    {
        fprintf(stderr, "Error: cannot set time.\n");
        return EXIT_FAILURE;
    }
    srand((unsigned)t);
    
    s_emulator emulator = {0};
    void (*opcode_functions[OPCODE_NB]) (s_emulator*, Uint8, Uint8, Uint8);
    
    if(0 != initialize_emulator(&emulator, opcode_functions))
    {
        destroy_emulator(&emulator);
        return EXIT_FAILURE;
    }
    if(0 != load_rom(&emulator.cpu, rom_filename))
    {
        destroy_emulator(&emulator);
        return EXIT_FAILURE;
    }
        
    emulate(&emulator, opcode_functions);
    destroy_emulator(&emulator);
    
    return 0;
}
