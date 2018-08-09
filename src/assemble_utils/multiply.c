#include "instruction_defs.h"
#include "assemblerImplementation.h"
#include "registers.h"
#include "multiply.h"

 uint32_t *multiply(instruction instr){

    MultiplyInstr_t mult;

     mult.Rn = assignReg(instr.Rn);
     mult.Rs = assignReg(instr.Rs);
     mult.Rm = assignReg(instr.Rm);

    if(instr.opcode == "mul"){
     mult.A = 0;
     mult.Rd = mult.Rm * mult.Rs;
     mult.mnemonic = mul;
   } else if(instr.opcode == "mla"){
       mult.A = 1;
       mult.Rd = mult.Rm * mult.Rs + mult.Rn;
       mult.mnemonic = mla;
    }
   mult.S = 0;
   mult.cond = AL;

   uint32_t binary = mult.Rn | 0x0090 | (mult.Rs << 8) |(mult.Rn << 12) | (mult.Rd << 16) | (mult.S << 20) |
           (mult.A << 21) | 0x00000000 | 0xE0000000;
   uint32_t *binPtr = &binary;
   return binPtr;
}