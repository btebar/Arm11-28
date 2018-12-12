

#include "emulate_utils/binaryloader.h"
#include "emulate_utils/execute.h"
#include <string.h>


word_t getBitRange(word_t num, int start, int length);

// Initialisation of the elements of the machine
CPU cpuo = {
  .registers = {0},
  .fetchedInstruction = 0,
};

MEMORY memoArray = {
  .memoryAlloc = {0},
};

//Dummy instruction to setup the pipeline
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
    //Check if it receives the correct number of arguments
    assert(argc == 2 && "Incorrect number of arguments");

    char *givenFile = argv[1];

    //Allocates the CPU
    CPU *cpu = (CPU *)malloc(sizeof(CPU));
    *cpu = cpuo;

    //Allocate the memory
    MEMORY *memory = (MEMORY *)malloc(sizeof(MEMORY));
    *memory = memoArray;

    (cpu->decodedInstruction) = (instructions *)malloc(sizeof(instructions));
    *(cpu->decodedInstruction) = NullInstruction;

    //Allocate the machine
    MACHINE *machine = (MACHINE*)(malloc(sizeof(MACHINE)));
    //Set values
    (machine->c) = *cpu;
    (machine->mem) = *memory;

    //Load the file
    loadFile(givenFile,machine->mem.memoryAlloc);

    //Fill the pipeline before you begin
    word_t fetched = 0;
    word_t address = machine->c.registers[PC];
    fetched |= ((uint32_t) machine->mem.memoryAlloc[binToDec(address)]);

    machine->c.fetchedInstruction = fetched;
    machine->c.instructionIsFetched=true;

    machine->c.registers[PC] += 1;
    if(machine->c.instructionIsFetched){
      decode(machine);
    }

    // --------------------MAIN WHILE LOOP---------------------------

    //Once there are instructions to fetch, execute and decode it will execute the loop
    while (machine->c.decodedInstruction->type != Halt  ) {
      //Fetch
      address = machine->c.registers[PC];
      fetched = 0;
      fetched |= ((uint32_t) machine->mem.memoryAlloc[address]);
      machine->c.instructionIsFetched = true;
      machine->c.fetchedInstruction = fetched;

      //Execute
      if (machine->c.decodedInstruction->type != None) {
      execute(machine);
      }


      //Decode
      *(machine->c.decodedInstruction) = NullInstruction; //dummy value to be defined by decode
      if(machine->c.instructionIsFetched){ //check if the instruction has to be decoded
        decode(machine);
      }

      machine->c.registers[PC] += 1; // as we made 4-bytes addresses the value
                                     // of the PC is only incremented by 1
    }
    machine->c.registers[PC] += 1; // last time incremented
    execute(machine); // executes halt and prints the values held in memory and registers

    //Frees and exits the program
    free(machine->c.decodedInstruction);
    free(cpu);
    free(memory);
    free(machine);
    return EXIT_SUCCESS;
  }
