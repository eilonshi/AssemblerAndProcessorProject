#include "state.h"

stateStruct state;

void initialize_state()
{
    state = (stateStruct){
        .pc = 0,
        .instruction = 0,
        .rt = 0,
        .rs = 0,
        .rd = 0,
        .imm = 0,
        .opcode = 0,
        .isActiveIRQ = 0,
        .timer_disk = 0,
        .next_irq2 = 0,
        .registers = {0},
        .ioRegisters = {0},
        .memory = {0},
        .disk = {{0}},
        .monitor = {{0}}};
}