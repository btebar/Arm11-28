//
// Created by nahida on 6/6/18.
//

#ifndef ARM11_28_REGISTERS_H
#define ARM11_28_REGISTERS_H
#include<stdint.h>
#include<stdlib.h>

typedef enum {
    r0 = 0000,
    r1 = 0001,
    r2= 0010,
    r3 = 0011,
    r4 = 0100,
    r5 = 0101,
    r6 = 0110,
    r7 = 0111,
    r8 = 1000,
    r9 = 1001,
    r10 = 1010,
    r11 = 1011,
    r12 = 1100,
    r15 = 1101,
    r16 = 1110,

}registers;

uint8_t assignReg(char *reg){
    uint8_t regstr = NULL;

 if(!strcmp(reg, "r0")){
     regstr = r0;
 }
 if(!strcmp(reg, "r1")){
     regstr = r1;
 }
 if(!strcmp(reg, "r2")){
     regstr = r2;
 }
 if(!strcmp(reg, "r3")){
     regstr = r3;
 }
if(!strcmp(reg, "r4")){
    regstr = r4;
}
if(!strcmp(reg, "r5")){
    regstr = r5;
}
if(!strcmp(reg, "r6")){
    regstr = r6;
}
if(!strcmp(reg, "r7")){
    regstr = r7;
}
if(!strcmp(reg, "r8")){
    regstr = r8;
}
if(!strcmp(reg, "r9")){
    regstr = r9;
}
if(!strcmp(reg, "r10")){
    regstr = r10;
}
if(!strcmp(reg, "r11")){
    regstr = r11;
}
if(!strcmp(reg, "r12")){
    regstr = r12;
}
if(!strcmp(reg, "r15")){
    regstr = r15;
}
if(!strcmp(reg, "r16")){
    regstr = r16;
}
return regstr;
}



#endif //ARM11_28_REGISTERS_H
