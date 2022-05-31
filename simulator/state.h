#include "global.h"

// Structs:
typedef struct stateStruct
{
    int pc;
    int instruction;
    int imm;
    int rt;
    int rs;
    int rd;
    int clockCycles;
    int isActiveIRQ;
    int memory[MEMORY_SIZE];
    int registers[NUM_REGISTERS];
    int ioRegisters[NUM_IO_REGISTERS];
    int memory[MEMORY_SIZE];

} stateStruct;
