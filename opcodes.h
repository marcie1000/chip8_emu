#ifndef OPCODES_H
#define OPCODES_H

#include <SDL.h>

#include "emulator.h"

#define UNUSED __attribute__((unused))

extern void opcode_unimplemented(s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1);
                     
extern void opcode_0NNN(UNUSED s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1);//0                                                                                   //0NNN 0
extern void opcode_00E0(s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1);       //1
extern void opcode_00EE(s_emulator *emulator, UNUSED Uint8 b3, UNUSED Uint8 b2, UNUSED Uint8 b1);       //2
extern void opcode_1NNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N);                      //3
extern void opcode_2NNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N);                      //4
extern void opcode_3XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N);                      //5
extern void opcode_4XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N);                      //6
extern void opcode_5XY0(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_0);               //7
extern void opcode_6XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N);                      //8
extern void opcode_7XNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N);                      //9
extern void opcode_8XY0(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_0);               //10
extern void opcode_8XY1(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_1);               //11
extern void opcode_8XY2(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_2);               //12
extern void opcode_8XY3(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_3);               //13
extern void opcode_8XY4(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_4);               //14
extern void opcode_8XY5(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_5);               //15
extern void opcode_8XY6(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_Y, UNUSED Uint8 b1_6);        //16
extern void opcode_8XY7(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_7);               //17
extern void opcode_8XYE(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_Y, UNUSED Uint8 b1_E);        //18
extern void opcode_9XY0(s_emulator *emulator, Uint8 b3_X, Uint8 b2_Y, UNUSED Uint8 b1_0);               //19
extern void opcode_ANNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N);                      //20
extern void opcode_BNNN(s_emulator *emulator, Uint8 b3_N, Uint8 b2_N, Uint8 b1_N);                      //21
extern double random_double(void);
extern int rand_int(int min, int max);
extern void opcode_CXNN(s_emulator *emulator, Uint8 b3_X, Uint8 b2_N, Uint8 b1_N);                      //22
extern void opcode_DXYN(s_emulator *emulator, Uint8 b3_VX, Uint8 b2_VY, Uint8 b1_VN);                   //23
extern void opcode_EX9E(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_9, UNUSED Uint8 b1_E);        //24
extern void opcode_EXA1(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_A, UNUSED Uint8 b1_1);        //25
extern void opcode_FX07(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_0, UNUSED Uint8 b1_7);        //26
extern void opcode_FX0A(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_0, UNUSED Uint8 b1_A);        //27
extern void opcode_FX15(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_1, UNUSED Uint8 b1_5);        //28
extern void opcode_FX18(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_1, UNUSED Uint8 b1_8);        //29
extern void opcode_FX1E(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_1, UNUSED Uint8 b1_E);        //30
extern void opcode_FX29(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_2, UNUSED Uint8 b1_9);        //31
extern void opcode_FX33(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_3, UNUSED Uint8 b1_3);        //32
extern void opcode_FX55(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_5, UNUSED Uint8 b1_5);        //33
extern void opcode_FX65(s_emulator *emulator, Uint8 b3_X, UNUSED Uint8 b2_6, UNUSED Uint8 b1_5);        //34


#endif //OPCODES_H

