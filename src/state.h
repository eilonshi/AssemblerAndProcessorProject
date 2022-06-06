#include "global.h"

// Structs:
typedef struct stateStruct
{
    int pc;
    int instruction;
    int rt;
    int rs;
    int rd;
    int imm;
    int opcode;
    int isActiveIRQ;
    int timer_disk;
    int next_irq2;
    int stop_running;
    int registers[NUM_REGISTERS];
    int ioRegisters[NUM_IO_REGISTERS];
    int memory[MEMORY_SIZE];
    int disk[NUM_SECTORS][SIZE_SECTOR];
    int monitor[MONITOR_WIDTH][MONITOR_HEIGHT];
} stateStruct;

extern stateStruct state;

void initialize_state();
void update_monitor();
