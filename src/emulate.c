

#include "emulate_utils/binaryloader.h"
#include "emulate_utils/execute.h"
#include <string.h>


uint32_t getBitRange(uint32_t num, int start, int length);

CPU cpuo = {
  .registers = {0},
  .fetchedInstruction = 0,
};

MEMORY memoArray = {
  .memoryAlloc = {0},
};

instructions NullInstruction = {
 None,
 1111,
 2, //Random value not equal to any know cond codes
 0,

//registers
 -1,
 -1,
 -1,
 -1,

//flags for operations
 0,
 0,
 0,
 0,
 0,
 0,

//immediate value
 0,

//operand2
 0,
 0,
 (SHIFT_CODE) -1,
 -1
};


int main(int argc, char **argv) {
    /* explanation:
     * would it not be easier to just use the global variable machine from memImplementation.h rather than create a new
     * one and have to mess around with pointers and what not?
     * at the start of this function, main, just initialise everything to zero. doesn't mean the struct variable cant
     * exist before this, does it?
     *
     * global var may be changed elsewhere unknowingly
     *
    */

    /*for ( int i = 0 ; i < 17; ++i ) {
      machine.registers[i] = 0;
    }

    for ( int i = 0 ; i < 16384; ++i ) {
      machine.memoryAlloc[i] = 0;
    }*/

    // could use this to initialise to zero?


    //uint32_t *memArray = (uint32_t *) calloc(16384, sizeof(uint32_t));

    //uint32_t *registerArray = (uint32_t *) calloc(17, sizeof(uint32_t));
    assert(argc == 2 && "Incorrect number of arguments");

    char *givenFile = argv[1];
    //-------think is the correct way of initialising the machine---
    CPU *cpu = (CPU *)malloc(sizeof(CPU));

    *cpu = cpuo;

    MEMORY *memory = (MEMORY *)malloc(sizeof(MEMORY));

    *memory = memoArray;
    (cpu->decodedInstruction) = (instructions *)malloc(sizeof(instructions));
    *(cpu->decodedInstruction) = NullInstruction;



    MACHINE *machine = (MACHINE*)(malloc(sizeof(MACHINE)));

    (machine->c) = *cpu;
    (machine->mem) = *memory;

    //-------------------------------------
    /*(machine->mem.memoryAlloc) = &memArray;


    uint32_t *registerArray = (uint32_t *) calloc(17, sizeof(uint32_t));

      *(machine->c.registers) = &registerArray;*/
    // i'm assuming traversing through the array and then using calloc is redundant, but im still going to keep it in

    // read from binary file into memory array
    // i wonder if i could do this : loadFile(givenFile, memArray) -> technically it should be fine because they both point to memAlloc[0]?

    loadFile(givenFile,machine->mem.memoryAlloc);



    /* for the main while loop of emulate_utils:
     * is it better to create a struct for instructions, and then extract one uint32 from memory and use it, or is it
     * better to not do any such thing and just process the "instructions" in a 3-staggered while loop?
     * if instructions is it's own struct:
     *      then it can have data members instruction_type, cond and a bunch of others to store the effects and particulars
     *      of that particular instructions, but this may lead to problems with dereferencing later
     * if instructions is NOT it's own struct:
     *      then as soon as we extract from memory, there needs to be a helper function which extracts all the necessary
     *      information like instructionType, cond, opcode and other particulars of the instructions. could stick these
     *      functions into a .h and .c file of their own, or could have them inside the while loop which would make it
     *      quite messy
     *      ^^^ oh i was talking about decode here and in the point above.
     *
     * essentially, it comes down to having a helper functions to perform decode, or having decode within the struct
     * design decision here
     */

    //machine->c.decodedInstruction = (instructions *) malloc(sizeof(instructions)); //creates space for the decoded instructions
    //*(machine->c.decodedInstruction) = NullInstruction;

    // read from binary file into memory array
    // i wonder if i could do this : loadFile(givenFile, memArray) -> technically it should be fine because they both point to memAlloc[0]?


    // Fill the pipeline before you begin
    uint32_t fetched = 0;
    uint32_t address = machine->c.registers[PC];
    //uint32_t fetched = 0;
    //for(int i = 0; i < 4; i++) {
            fetched |= ((uint32_t) machine->mem.memoryAlloc[binToDec(address)]); //<< (i * 8);
      //  }



    //fetched = bswap_32(fetched);
    // flipped the bits here

    machine->c.fetchedInstruction = fetched;
    machine->c.instructionIsFetched=true;
    //memcpy(&machine->c.fetchedInstruction,&fetched, sizeof(fetched));
    machine->c.registers[PC] += 1;
    if(machine->c.instructionIsFetched){
      decode(machine);
    }

    // --------------------MAIN WHILE LOOP---------------------------

    //bool finalise = false; //finalise will become true when the instructions is the zero instructions: halt


    while (machine->c.decodedInstruction->type != Halt  ) {
      //fetch
      address = machine->c.registers[PC];
      fetched = 0;
      fetched |= ((uint32_t) machine->mem.memoryAlloc[binToDec(address)]);// << (i * 8);
      machine->c.instructionIsFetched = true;

      //fetched = bswap_32(fetched);
      //flippd the bits here

      machine->c.fetchedInstruction = fetched;

      //machine->c.registers[PC] += 1; // four bytes because is 4-byte addressable


      //execute
      if (machine->c.decodedInstruction->type != None) {
      execute(machine);
      }


      //decode
      *(machine->c.decodedInstruction) = NullInstruction;
      if(machine->c.instructionIsFetched){ //check if the instruction has to be decoded
        decode(machine);
      }

      machine->c.registers[PC] += 1; // four bytes because is 4-byte addresable

    }
    machine->c.registers[PC] += 1;
    execute(machine);
    free(machine->c.decodedInstruction);
    free(cpu);
    free(memory);
    free(machine);
    return EXIT_SUCCESS;
  }
