#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "instruction_defs.h"
#include "sdt.h"


uint32_t *SDTassembling(instruction inst){
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
    uint32_t progC = inst.memoryAddr + 8;
  if (strcmp(inst.opcode,ldr) == 0) {
      l_flag = 1;
      // L = 1 if its an LDR
      if (address[0] == '=') {
          p_flag = 1;
          value = convertToWriteableFormat(address);
          if ( value < 0xFF ) {
              inst.opcode = "mov";
              inst.operand2 = address;
              inst.Rn = inst.Rd;
              return dataProcessing(inst);
          } else {
              i_flag = 0;
              u_flag = 1;
              printf("PC is %d\n",progC);
              rn = 15;
              offset = calculateOffset(progC, value);
              *returnVal = condition << 28 | 1 << 26 | i_flag << 25 |
                p_flag << 24 | u_flag << 23 | l_flag << 20 | rn << 16 | rd << 12 | offset;
              return returnVal;
          }
      }
  } else if ( strcmp(inst.opcode,str) == 0 )  {
      l_flag = 0;
      u_flag = 1;
      p_flag = 1;
      rn = 15;
      offset = progC - finalInstAddr;
      finalInstAddr+=4;

  } else {
      printf("error");
      exit(-1);
  }

  strcpy(newAdd,address);
  // further tokenizing
  if (newAdd[strlen(newAdd)-1] == ']') {
      p_flag = 1;
  } else {
      p_flag = 0;
  }

  int i = 0;
  tempPtr = strtok(newAdd,",");

  for (; tempPtr != NULL; i++) {
      tokAdd[i] = tempPtr;
      tempPtr = strtok(NULL,",");
  }

  // for nested instructions

  switch(i) {
      case 1:
          i_flag = 0;
          offset = 0;
          u_flag = 1;
          tokAdd[0][strlen(tokAdd[0]) - 1] = '\0';
          inst.Rn = tokAdd[0];
          rn = convertToWriteableFormat(inst.Rn);
          break;
      case 2:
          if (tokAdd[1][0] == ' ') {
              tokAdd[1]++;
          }

          if (p_flag) {
              tokAdd[1][strlen(tokAdd[1]) - 1] = '\0';
          } else {
              tokAdd[0][strlen(tokAdd[0]) - 1] = '\0';
          }

          i_flag = !checkIfImmediate(tokAdd[1]);
          u_flag = 1;
          offset = convertToWriteableFormat(tokAdd[1]);
          if (isNeg(tokAdd[1])) {
              u_flag = 0 ;
              offset*=1;
          }

          inst.Rn = tokAdd[0];
          rn = convertToWriteableFormat(inst.Rn);
          break;
      case 3:
          base = tokAdd[2];
          shiftT = strtok(tokAdd[i]," ");
          shiftA = strtok(NULL," ");

          if (p_flag) {
              if(shiftA[strlen(shiftA) - 1] == ']') {
                  shiftA[strlen(shiftA) - 1] = '\0';
              }
          } else {
              if (base[strlen(base)-1] == ']') {
                  base[strlen(base)-1] = '\0';
              }
          }
          offset = shiftOperand(base, shiftT, shiftA);
          u_flag = 1;

          if (offset < 0) {
              u_flag = 0;
              offset*=-1;
          }
          i_flag = !checkIfImmediate(base);
          inst.Rn = tokAdd[0];
          rn = convertToWriteableFormat(inst.Rn);
  }
  *returnVal = condition << 28 | 1 << 26 | i_flag << 25 | p_flag << 24 |
   u_flag << 23 | l_flag << 20 | rn << 16 | rd << 12 | offset;

  return returnVal;
}
