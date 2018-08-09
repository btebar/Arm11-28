#include <stdio.h>
#include <stdint.h>
#include "instruction_defs.h"
#include "registers.h"

uint32_t *SDTassembling(instruction inst){
  /*SDTinstr_t SDTinstr;
  SDTinstr.cond = AL;
  if (!strcmp(inst.opcode, "ldr")) {
    SDTinstr.L = '1'; //load bit set
  } else {
    SDTinstr.L = '0';
  }
  //checks if its the form of <=expression>
  if(inst.address[0] == '=') {
    char *expr = inst.address + 1;
    if (atoi(expr) <= 0xFF) { //the value to be load fits in a mov instruction
      instruction movInstr;
      movInstr.opcode = "mov";
      movInstr.memoryAddr = inst.address;
      movInstr.type = DATA_PROCESSING;
      movInstr.Rd = inst.Rd;
      movInstr.operand2 = inst.address;
      return dataProc(movInstr);
    } else { // stores the value at the end of the assemble file and gets the address
      SDTinstr.P = '1';
      int NUMofLines= numberOfLinesInSource;

      SDTinstr.Rn = r16; //this should be r15 bc its the PC-----
      SDTinstr.U = '0';
      SDTinstr.I = '1';
      // the address of the value is going to be the actual address adding the number of lines multiplied by 4 so its unique
      SDTinstr.offset = inst.memoryAddr + (NUMofLines*4);
      //|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
      //have to save the value in the destination file (no clue of how to do that)
      //|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    }
  }else {
    SDTinstr.Rn = assignReg(inst.Rn);
    SDTinstr.offset = inst.expression;

    SDTinstr.P = inst.PFlag; //sets the P bit if the tokenizeHelper decoded an address of the form [Rn, <#expression>]
  }
*/
  char *address = inst.address;
  char *newAdd = malloc(sizeof(char)*strlen(address));

  char *tempPtr;

  const char ldr[] = "ldr";
  const char str[] = "str";
  bool p_flag, i_flag, u_flag, l_flag;

  int value = 1;

  uint16_t rd = convertToWriteableFormat(inst.Rd);
  uint16_t rn;
  int32_t offset ;
  uint16_t condition = 14;
  char *tokAdd[3], *base, *shiftT, *shiftA;
  uint32_t *returnVal = malloc(sizeof(uint32_t));

  if (strcmp(inst.opcode,ldr) != 0) {
      l_flag = 1;
      if (address[0] == '=') {
          p_flag = 1;
          value = convertToWriteableFormat(address);
          if ( value < 0xFF ) {
              inst.opcode = "mov";
              inst.operand2 = address;
              inst.Rn = '#0';
              return dataProcessing(inst);
          } else {
              i_flag = 0;
              u_flag = 1;
              uint32_t progC = inst.memoryAddr  + 8;
              printf("PC is %d\n",progC);
              rn = 15;
              offset = findConstOffset(progC, value, constTableHead);
              *returnVal = condition << 28 | 1 << 26 | i_flag << 25 | p_flag << _ | l_flag << 20 | rn << 16 | rd << 12 | offset;
                // CHECK
              return returnVal;
          }
      }
  } else if ( strcmp(inst.opcode,str) != 0 )  {
      l_flag =0;
  } else {
      printf("error");
      exit(EXIT_FAILURE);
  }

  strcpy(newAdd,address);
  // second run at tokenising
  if (newAdd[strlen(newAdd)-1] == ']') {
      p_flag = 1;
  } else {
      p_flag = 0;
  }

  int i = 0;
  tempPtr = strtok(newAdd,",");

  while (tempPtr) {
      tokAdd[i] = tempPtr;
      i++;
      tempPtr = strtok(NULL,",");
  }

  switch(i) {
      case 1:
          i_flag = 0;
          offset = 0;
          u_flag = 0;
          tokAdd[0][strlen(tokAdd[0])-1] = '\0';
          inst.Rn = tokAdd[0];
          rn = convertToWriteableFormat(inst.Rn);
          break;
      case 2:
          if (tokAdd[1][0] == ' ') {
              tokAdd[1]++;
          }

          if (p_flag) {
              tokAdd[1][strlen(tokAdd[1])-1] = '\0';
          } else {
              tokAdd[0][strlen(tokAdd[0])-1] = '\0';
          }

          i_flag = !checkIfImmediate(tokAdd[i]);
          u_flag = 1;
          offset = convertToWriteableFormat(tokAdd[i]);
          if (offset < 0) {
              u_flag =0 ;
              offset*=1;
          }

          inst.Rn = tokAdd[0];
          rn = convertToWriteableFormat(inst.Rn);
          break;
      case 3:
          base = tokAdd[i];
          shiftT = strtok(tokAdd[2]," ");
          shiftA = strtok(NULL," ");

          if (p_flag) {
              if(shiftA[strlen(shiftA)-1]==']') {
                  shiftA[strlen(shiftA)-1] = '\0';
              }
          } else {
              if (base[strlen(base)-1] == ']') {
                  base[strlen(base)-1] = '\0';
              }
          }
          offset = shiftOperand(base,shiftT,shiftA);
          u_flag = 1;

          if ( offset < 0 ) {
              u_flag = 0;
              offset*=-1;
          }
          i_flag = !checkIfImmediate(base);
          inst.Rn = tokAdd[0];
          rn = convertToWriteableFormat(inst.Rn);
  }

  *returnVal = cond << 28 | 1 << 26 | i_flag << 25 | p_flag << 24 | u_flag << ? | l_flag << 20 | rn << 16 | rd << 12 | offset;
  return returnVal;


}
