#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "chip8_cpu.h"
#include "chip8_graphics.h"
#include "emulator.h"

void update_event(s_input *input)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case(SDL_QUIT):
                input->quit = SDL_TRUE;
                break;
            case(SDL_KEYDOWN):
                input->key[event.key.keysym.scancode] = SDL_TRUE;
                break;
            case(SDL_KEYUP):
                input->key[event.key.keysym.scancode] = SDL_FALSE;
                break;
            case(SDL_MOUSEMOTION):
                input->x = event.motion.x;
                input->y = event.motion.y;
                input->xrel = event.motion.xrel;
                input->yrel = event.motion.yrel;
                break;
            case(SDL_MOUSEWHEEL):
                input->xwheel = event.wheel.x;
                input->ywheel = event.wheel.y;
                break;
            case(SDL_MOUSEBUTTONDOWN):
                input->mouse[event.button.button] = SDL_TRUE;
                break;
            case(SDL_MOUSEBUTTONUP):
                input->mouse[event.button.button] = SDL_FALSE;
                break;
            case(SDL_WINDOWEVENT):
                if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                    input->resize = SDL_TRUE;
//                else
//                    input->resize = SDL_FALSE;
                break;
        }
    }
}

int initialize_SDL(s_emulator *emulator)
{
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        fprintf(stderr, "Error SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    if(0 != Mix_Init(0))
    {
        fprintf(stderr, "Error Mix_Init: %s\n", Mix_GetError());
        return EXIT_FAILURE;
    }
    
    if(0 != Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 1024))
    {
        fprintf(stderr, "Error Mix_OpenAudio: %s\n", Mix_GetError());
        return EXIT_FAILURE;
    }
    
    emulator->bip = Mix_LoadWAV("bip.wav");
    if(emulator->bip == NULL)
    {
        fprintf(stderr, "Error Mix_LoadWAV: %s\n", Mix_GetError());
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

void destroy_SDL(s_emulator *emulator)
{
    if(NULL != emulator->bip)
        Mix_FreeChunk(emulator->bip);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

int initialize_emulator(s_emulator *emulator, void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8))
{
    initialize_cpu(&emulator->cpu);
    memset(&emulator->in, 0, sizeof(s_input));
    if(0 != initialize_SDL(emulator))
        return EXIT_FAILURE;
    if(0 != initialize_screen(&emulator->screen))
        return EXIT_FAILURE;
    init_memory_fonts(emulator->cpu.memory);
    init_opcodes_pointers(opcode_functions);
    init_key_table(emulator->key_table);
    return EXIT_SUCCESS;
}

void destroy_emulator(s_emulator *emulator)
{
    destroy_screen(&emulator->screen);
    destroy_SDL(emulator);
}

void play_sound(s_emulator *emulator)
{
    if(emulator->cpu.sound_counter > 0)
    {
        if(0 != Mix_Playing(0))
            Mix_PlayChannel(0, emulator->bip, 1);
        //emulator->cpu.sound_counter = 0;
    }
}

void emulate(s_emulator *emulator, void (*opcode_functions[OPCODE_NB])(s_emulator*, Uint8, Uint8, Uint8))
{
    while(!emulator->in.quit)
    {
        update_event(&emulator->in);
        manage_input(emulator);
        if(emulator->in.resize)
        {
            resize_screen(&emulator->screen);
            emulator->in.resize = SDL_FALSE;
        }
        
        //OPERATIONS
        interpret(emulator, opcode_functions);
        interpret(emulator, opcode_functions);
        interpret(emulator, opcode_functions);
        interpret(emulator, opcode_functions);

        
        play_sound(emulator);
        count(&emulator->cpu);
        update_screen(&emulator->screen);
        SDL_Delay(FPS_DELAY);
    }
}

void init_key_table(int *table)
{
    table[0] = SDL_SCANCODE_KP_0;
    table[1] = SDL_SCANCODE_KP_7;
    table[2] = SDL_SCANCODE_KP_8;
    table[3] = SDL_SCANCODE_KP_9;
    table[4] = SDL_SCANCODE_KP_4;
    table[5] = SDL_SCANCODE_KP_5;
    table[6] = SDL_SCANCODE_KP_6;
    table[7] = SDL_SCANCODE_KP_1;
    table[8] = SDL_SCANCODE_KP_2;
    table[9] = SDL_SCANCODE_KP_3;
    table[0xA] = SDL_SCANCODE_RIGHT;
    table[0xB] = SDL_SCANCODE_KP_PERIOD;
    table[0xC] = SDL_SCANCODE_KP_MULTIPLY;
    table[0xD] = SDL_SCANCODE_KP_MINUS;
    table[0xE] = SDL_SCANCODE_KP_PLUS;
    table[0xF] = SDL_SCANCODE_KP_ENTER;
}

void manage_input(s_emulator *emulator)
{
    s_cpu *cpu = &emulator->cpu;
    s_input *in = &emulator->in;
    
    for(size_t i = 0; i < NB_KEYS; i++)
        cpu->keys[i] = in->key[emulator->key_table[i]];
}

