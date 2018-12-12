#include "execute.h"

//declaration of the methods
void printBitsofMem(word_t memAlloc);
void printBitsofReg(int32_t reg, bool isPC, bool isCPSR);

int checkCondition(MACHINE *machine) {
    //checking whether the condition set in the cond field
    //of the instructions correspond to the flags of the CPSR

    int cpsrFlags = machine->c.registers[CPSR] >> condBits; //getting the four last bits of the CPSR

    switch(machine->c.decodedInstruction->cond) {
        case 0:
            return cpsrFlags&Z_MASK; // equal
        case 1:
            return !(cpsrFlags&Z_MASK); //not equal
        case 10:
            return (cpsrFlags&N_MASK) == ((cpsrFlags&V_MASK) >> 3); //greater or equal
        case 11:
            return ((cpsrFlags&N_MASK) != ((cpsrFlags&V_MASK) >> 3)); //less than
        case 12:
            return (!(cpsrFlags&Z_MASK) && (cpsrFlags&N_MASK) ==
              ((cpsrFlags&V_MASK) << 3)); // greater than
        case 13:
            return (cpsrFlags&Z_MASK) || ((cpsrFlags&N_MASK) !=
              ((cpsrFlags&V_MASK) << 3)); //less than or equal
        case 14:
            return 1; //always
        default:
            exitProgr(machine);
            return 0;
    }


}

void execute_MulI(MACHINE *machine){
    //simple multiplication
    word_t result = machine->c.registers[machine->c.decodedInstruction->Rm] *
            machine->c.registers[machine->c.decodedInstruction->Rs];

    //if A is set, then add accumulator
    if (machine->c.decodedInstruction->A) {
        result += machine->c.registers[machine->c.decodedInstruction->Rn];
    }

    machine->c.registers[machine->c.decodedInstruction->Rd] = result;

    //if S is set, update CPSR flag
    if (machine->c.decodedInstruction->S) {

        //N will be updated to the last bit of the result, rest of CPSR stays the same
        machine->c.registers[CPSR] &= 0x7FFFFFFF;
        machine->c.registers[CPSR] |= (result>>OTHERSIDE) * N_MASK_32;

        //if result is zero, Z bit is set and the rest of CPSR stays the same
        if (result == 0) {
            machine->c.registers[CPSR] &= 0xBFFFFFFF;
            machine->c.registers[CPSR] = Z_MASK_32 | machine->c.registers[CPSR];
        }
    }
}


void execute_Halt(MACHINE *machine){
    //printing the value of each register to standard output
    printf("Registers:\n");
    bool isPC = false;
    bool isCPSR = false; //CPSR needs a different padding
    for(int i = 0; i < NUM_REGISTERS; i++) {
      isPC = false;
      isCPSR = false;
      if (!(i == UnusedReg1 || i == UnusedReg2)) {
        if (i == PC) {
          printf("PC  :");
          isPC = true;
        } else if(i == CPSR) {
          printf("CPSR:");
          isCPSR = true;
        } else {
        printf("$%-3i:", i);
        }
        printBitsofReg(machine->c.registers[i], isPC, isCPSR);
      }
    }
    printf("Non-zero memory:\n");
    for (int i = 0; i < NUM_ADDRESSES; i++) {
      if (machine->mem.memoryAlloc[i] != 0) {
        printf("0x%08x: ", i * 4);
        printBitsofMem(machine->mem.memoryAlloc[i]);
      }
    }
}



//prints bit sequence of register
void printBitsofReg(int32_t reg, bool isPC, bool isCPSR) {

    if (isPC) {
      printf("% 11d (0x%08x)\n", reg*4, reg*4); //4-byte addressable
    } else if (isCPSR && (reg < 0)) {
      printf("% 12d (0x%08x)\n", reg, reg);
    } else if (reg < 0) {// if the number is ngative we need more space
      if (reg <= (-2147483648)) { //bound for the padding
        printf("% 12d (0x%08x)\n", reg, reg);
      } else {
        printf("%  11d (0x%08x)\n", signedtwos_to_unsigned(reg), reg);
      }
    } else {
    printf("% 11d (0x%08x)\n", reg, reg);
    }
}

void printBitsofMem(word_t memAlloc) {
      // swapping bits because of the endianess of the memory
      word_t mem = bswap_32(memAlloc);
      printf("0x%08x\n", mem);
}


//checks for negative value and turns offset into positive binary
word_t signedtwos_to_unsigned(int32_t signednum){
    word_t signednumAbs = (~signednum) + 1;
    if (signednum >> OTHERSIDE) {
        signednumAbs *= -1;
    }
    return signednumAbs;
}

word_t signedtwos_to_int(int32_t signednum){
    word_t  signednumAbs = (~signednum) + 1;
    if (signednum >> OTHERSIDE) {
        signednumAbs *= -1;
    }
    return signednumAbs;
}

void execute_branch(MACHINE *machine){
    machine->c.instructionIsFetched = false; //ignoring last instruction;
    int32_t offset = (machine->c.decodedInstruction->offset);  // /4 - 1;
    machine->c.registers[PC] += (offset)/4;
}

//Operand are the 12 last bits of the instruction (although it is passed as an uin32_t)
word_t shiftReg(word_t operand, MACHINE *machine) {
    int amount; //the number of positions to be shifted
    int32_t mask2; // mask that we are gonna use later
    if ((operand & 0x10) != 0) {  //checks if it's shifted by constant amount
                                  //or by the amount stored in Rs
        // amount stored in las byte of Rs
        amount =
        getBitRange(machine->c.registers[getBitRange(operand, BYTE_1, bits_4)],
                                                      0, BYTE_1);
    } else {
      // takes the five bits from bit 7 as a constant value
        amount = getBitRange(operand, 7, 5);
    }
    int32_t value = machine->c.registers[machine->c.decodedInstruction->Rm];
    switch(getBitRange(operand, 5, 2)) { // bits that determine the type of shift
        case 0: //logical shift left
            machine->c.decodedInstruction->shift = LSL; //sets the type of switch
            if (machine->c.decodedInstruction->S) { // carry flag changes
              // mask to get the LSB in the original value we are gonna shift later
              int32_t mask = 1 << (BYTES_4 - amount);
              // mask that sets to zero the carry bit so we can set it later
              machine->c.registers[CPSR] &= 0xDFFFFFFF;
              //takes the bit we need (for setting C in CPSR)
              // and shifts it to its correct position in CPSR
              mask2 = (value & mask) << Cbit;
              machine->c.registers[CPSR] |= mask2; //ors, to set the CPSR C bit
            }
            value <<= amount; //shifts
            break;
        case 1: //logical shift right
            machine->c.decodedInstruction->shift = LSR;//sets the type of switch
            if (machine->c.decodedInstruction->S) { // carry flag changes
                // mask to get the LSB in the original value we are gonna shift later
                int32_t mask = 1 << (amount);
                // mask that sets to zero the carry bit so we can set it later
                machine->c.registers[CPSR] &= 0xDFFFFFFF;
                //takes the bit we need (for setting C in CPSR)
                // and shifts it to its correct position in CPSR
                mask2 = (value & mask) << Cbit;
                machine->c.registers[CPSR] |= mask2; //ors, to set the CPSR C bit

            }
            value >>= amount;
            break;
        case 2: //arithmetic shift right
            machine->c.decodedInstruction->shift = ASR; //type of shift
            if (machine->c.decodedInstruction->S) { // CPSR would be altered
                int32_t mask = 1 << (amount);
                machine->c.registers[CPSR] &= 0xDFFFFFFF;
                mask2 = (value & mask) << Cbit;
                machine->c.registers[CPSR] |= mask2;
            }
            char signBit = (value & 80000000) >> OTHERSIDE; //takes bit 31
            word_t maskASR = 0;
            //creates a mask for the bits on the left with the value of signbit
            for (int i = 0; i < amount; i++) {
                maskASR |= signBit << (31-i);
            }
            //sets the bits on the left with signBit
            value = (value >> amount) | maskASR;
            break;
        case 3: // rotate shift right
            machine->c.decodedInstruction->shift = ASR;
            if (machine->c.decodedInstruction->S) {
                int32_t mask = 1 << (amount);
                machine->c.registers[CPSR] &= 0xDFFFFFFF;
                mask2 = (value & mask) << Cbit;
                machine->c.registers[CPSR] |= mask2;
            }
            word_t maskRotate = 0;
            // creates a mask to get the last (amount) bits of the value
            for (int i = 0; i < amount; i++) {
              maskRotate |= 1 << i;
            }
            //right bits of the shifted value
            maskRotate <<= 27;
            value = (value >> amount) | maskRotate;
            break;
        default: break;
    }
    return value;
}

//rotates with right shifts and takes number of rotations as parameter
word_t rotate(word_t operand, int numberRot){
    int num = (numberRot);  //number of rotations
    for (int i = 0; i < num; i++) {
        int firstBit = getBitRange(operand, 0, 1);
        operand >>= 1;
        firstBit <<= OTHERSIDE;
        operand |= firstBit;
    }
    return operand;
}

//turns a binary number into a decimal
int binToDec(int n) //checked
{
    int number = 0;
    int i = 0;
    int remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        number += remainder*(1 << i);
        ++i;
    }
    return number;
}

// gets a value from memory given the address
word_t getFromMemory(word_t address, MACHINE *machine) {
  int addr = (int) address/bits_4;
  word_t value = 0;
  switch (address % bits_4) { //odifferent cases for accessing the different
                        // bytes contained in a 4-byte addresable memory
    case 0:
      value = machine->mem.memoryAlloc[addr];
      break;
    case 1:
      value = (machine->mem.memoryAlloc[addr] & 0xFFFFFF00) >> BYTE_1;
      value |= (machine->mem.memoryAlloc[addr+1] & 0x000000FF) << BYTES_3;
      break;
    case 2:
      value = (machine->mem.memoryAlloc[addr] & 0xFFFF0000) >> BYTES_2;
      value |= (machine->mem.memoryAlloc[addr+1] & 0x0000FFFF) << BYTES_2;
      break;
    case 3:
      value = (machine->mem.memoryAlloc[addr] & 0xFF000000) >> BYTES_3;
      value |= (machine->mem.memoryAlloc[addr+1] & 0x00FFFFFF) << BYTE_1;
      break;
    default:
    break;
  }
  return value;
}

void setMemory(int address, word_t value, MACHINE *machine) {
int addr = (int) address/bits_4;
  switch (address % bits_4) {
    case 0:
      machine->mem.memoryAlloc[addr] = value;
      break;
    case 1:
        (machine->mem.memoryAlloc[addr]) &= 0x000000FF;
        (machine->mem.memoryAlloc[addr]) |= ((value <<BYTE_1) & 0xFFFFFF00);
        (machine->mem.memoryAlloc[addr+1]) &= 0xFFFFFF00;
        (machine->mem.memoryAlloc[addr+1]) |= ((value>>BYTES_3) & 0xFF);
      break;
    case 2:
        (machine->mem.memoryAlloc[addr]) &= 0x0000FFFF;
        (machine->mem.memoryAlloc[addr]) |= ((value<<BYTES_2) & 0xFFFF0000);
        (machine->mem.memoryAlloc[addr+1]) &= 0xFFFF0000;
        (machine->mem.memoryAlloc[addr+1]) |= ((value>>BYTES_2) & 0xFFFF);
      break;
    case 3:
        (machine->mem.memoryAlloc[addr]) &= 0x00FFFFFF;
        (machine->mem.memoryAlloc[addr]) |= ((value<<BYTES_3) & 0xFF000000);
        (machine->mem.memoryAlloc[addr+1]) &= 0xFF000000;
        (machine->mem.memoryAlloc[addr+1]) |= ((value>>BYTE_1) & 0xFFFFFF);
      break;
    default:
    break;
  }
}

bool isGPIO (uint32_t number, MACHINE *machine) {
    switch(number) {
        case 0x20200008:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            machine->c.registers[machine->c.decodedInstruction->Rd] = number;
            break;
        case 0x20200004:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            machine->c.registers[machine->c.decodedInstruction->Rd] = number;
            break;
        case 0x20200000:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            machine->c.registers[machine->c.decodedInstruction->Rd] = number;
            break;
        case 0x20200028:
            printf("PIN OFF\n");
            break;
        case 0x2020001c:
            printf("PIN ON\n");
            break;
        default:
            return false;
    }
    return true;

}

void execute_SDT(MACHINE *machine) {
    //we must ensure that if Rn is the PC it contains the instruction's address
    // +8 bits bc of the pipeline
   if ((machine->c.decodedInstruction->Rn == PC) &&
        (machine->c.decodedInstruction->binary) !=
          machine->mem.memoryAlloc[(machine->c.registers[PC] - 1)]){

        fprintf(stderr, "PC doesn't contain the correct instructions");
        exit(EXIT_FAILURE);
    }
    machine->c.registers[PC] = (machine->c.registers[PC]+1)*bits_4;
    int32_t offsetValue; //declaring the value of the offset
    //if I set then the offset is a shifted reg
    if (machine->c.decodedInstruction->I) {
        //taking the bin code of the register (NOT ITS VALUE)
        int shifterReg =
              getBitRange(machine->c.decodedInstruction->binary, 0, bits_4);
        //Rm is the register which holds the value that is gonna be shifted
        machine->c.decodedInstruction->Rm = shifterReg;
        //shiftReg shifts the value in held in Rm
        //sets the offsetValue to the value that has been just shifted
        offsetValue =
              shiftReg(machine->c.decodedInstruction->offset & 0xFFF, machine);
    } else { //immediate offset (12 bits)
        offsetValue = (machine->c.decodedInstruction->offset) & 0xFFF;
    }
    word_t newAddress; //address where to store/load the value
    if (machine->c.decodedInstruction->U) { //U set then the values of base
                                            //register and offset are added
        newAddress = machine->c.registers[(machine->c.decodedInstruction->Rn)] +
                     offsetValue;
    } else { //subtraction
        newAddress = machine->c.registers[(machine->c.decodedInstruction->Rn)] -
                     offsetValue;
    }
     // out of bounds memory

    if(!(machine->c.decodedInstruction->P) &&
    (machine->c.registers[machine->c.decodedInstruction->Rn] >= MEM_BOUNDS) &&
            !isGPIO(machine->c.registers[machine->c.decodedInstruction->Rn],machine)) {
      word_t address = machine->c.registers[machine->c.decodedInstruction->Rn];
      printf("Error: Out of bounds memory access at address 0x%08x\n",
                                                                address);
    } else {

        if ( !isGPIO(newAddress,machine) && newAddress >= MEM_BOUNDS ) {
            printf("Error: Out of bounds memory access at address 0x%08x\n",
                   (newAddress&0x3FFFFFFF));
        }

      if (newAddress < MEM_BOUNDS) {
          if (machine->c.decodedInstruction->L) { //L set the load, otherwise store
              isGPIO(machine->mem.memoryAlloc[offsetValue],machine);
              if (machine->c.decodedInstruction->P) { // P set then pre-indexing
                  //stores the value of mem[address] in Rd
                  machine->c.registers[binToDec(machine->c.decodedInstruction->Rd)]
                          = getFromMemory(newAddress, machine);
              } else { //Post-indexing (after transferring the data)
                  word_t rn = machine->c.registers[machine->c.decodedInstruction->Rn];
                  machine->c.registers[binToDec(machine->c.decodedInstruction->Rd)]
                          = getFromMemory(rn, machine);
                  machine->c.registers[binToDec(machine->c.decodedInstruction->Rn)]
                          = newAddress;
              }
          } else {
              word_t value
                      = machine->c.registers[binToDec(machine->c.decodedInstruction->Rd)];
              if (machine->c.decodedInstruction->P) {
                  setMemory(newAddress, value, machine);
              } else {
                  //first loads from the address held in Rn
                  word_t address
                          = machine->c.registers[(machine->c.decodedInstruction->Rn)];
                  setMemory(address, value, machine);
                  //changes the value of Rn by offset
                  machine->c.registers[machine->c.decodedInstruction->Rn] =
                          newAddress;
              }
          }
      }
    }
    machine->c.registers[PC] = (machine->c.registers[PC]/bits_4)-1;

}



void execute_DPI(MACHINE *machine){
    instructions *instr = machine->c.decodedInstruction;
    word_t result;
    bool carry = 0;
    word_t flag = 0;
    word_t op2;
    int32_t RN = machine->c.registers[binToDec(instr->Rn)];

    if (instr->I) { //checking if operand2 is immediate value or shifted register
        op2 = getBitRange((instr->operand2 | 0x00000000), 0, BYTE_1);
        int numberRot = getBitRange(instr->binary, BYTE_1, bits_4) << 1;
        op2 = rotate(op2, numberRot); //rotates immediate value
    }
    else { //shifts register value
        op2 = shiftReg(instr->operand2, machine);
        carry = instr->carry;
    }

    switch (machine->c.decodedInstruction->opcode) {
        case 0: // and
        case 8: // tst
            result = RN & op2;
            flag = carry * C_MASK;
                    break;
        case 1: // eor
        case 9: // teq
            result = RN ^ op2;
            flag = carry * C_MASK;
            break;
        case 2: // sub
        case 10: // cmp
            result = RN + ((~op2) + 1);
            flag = (C_MASK_32) *
            ((RN >> OTHERSIDE) == (op2 >> OTHERSIDE) != (result >> OTHERSIDE));
                break;
        case 3: // rsb
            result = op2 + ((~RN) +1);
            flag = (C_MASK_32) *
            ((RN >> OTHERSIDE) == (op2 >> OTHERSIDE) != (result >> OTHERSIDE));
                break;
        case 4: // add
            result = RN + op2;
            flag =(C_MASK_32) *
            ((RN >> OTHERSIDE) == (op2 >> OTHERSIDE) != (result >> OTHERSIDE));
                break;
        case 12: // orr
            result = RN | op2;
            flag = carry * (C_MASK_32);
                break;
        case 13: // mov
            result = op2;
            flag = carry * (C_MASK_32);
                break;
        default:
            result = 0;
            exitProgr(machine);
        break;
    }

    //if one of these instructions, result is not written
    if (!(instr->opcode == BYTE_1 || instr->opcode == 9 || instr->opcode == 10)) {
        machine->c.registers[binToDec(machine->c.decodedInstruction->Rd)] =
                                                                        result;
    }

    //if S flag is set, CPSR flags have to be set in the following way
    if (instr->S) {
        // if result is all zeros, Z bit will be set
        if (result == 0) {
            machine->c.registers[CPSR] |= Z_MASK_32;
        } else {
            machine->c.registers[CPSR] &= 0xBFFFFFFF;
        }

        // N bit will be set to bit 31 of result
        machine->c.registers[CPSR] &= 0x7fffffff;
        machine->c.registers[CPSR] |=
                                  ((result >> OTHERSIDE) * N_MASK) << Nposition;

        //flag C is set to flag value but shifted to right position
        machine->c.registers[CPSR] &= 0xDFFFFFFF;
        machine->c.registers[CPSR] |= (flag);


    }
}

//depending on the instruction's type, executes a method
void execute(MACHINE *machine) {
    if (checkCondition(machine)) {
        switch (machine->c.decodedInstruction->type) {
            case Halt:
                execute_Halt(machine);
                break;
            case DProc:
                execute_DPI(machine);
                break;
            case SDT:
                execute_SDT(machine);
                break;
            case Mult:
                execute_MulI(machine);
                break;
            case Branch:
                execute_branch(machine);
                break;
            default:
                fprintf(stderr, "invalid instructions");
        }

    }
}
