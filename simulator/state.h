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
    int timer_disk;
    int registers[NUM_REGISTERS];
    int ioRegisters[NUM_IO_REGISTERS];
    int memory[MEMORY_SIZE];
    int disk[NUM_SECTORS][SIZE_SECTOR];
    int Monitor[MONITOR_WIDTH][MONITOR_HEIGHT];
} stateStruct;
