#include <stdio.h>
#include <stdlib.h>
#include "ops.h"
#include "global.h"
#include "files.h"
#include "interrupts.h"
#include "state.h"

void fetch()
{
    state.instruction = state.memory[state.pc];
    UpdateTimer();
}

void decode()
{
    state.rt = get_rt();
    state.rs = get_rs();
    state.rd = get_rd();
    state.opcode = get_opcode();

    if (state.rt == IMM_REG_NUM || state.rs == IMM_REG_NUM || state.rd == IMM_REG_NUM)
    {
        state.imm = state.memory[state.pc + 1];
        UpdateTimer();
        state.registers[IMM_REG_NUM] = get_imm();
    }
    else
        state.registers[IMM_REG_NUM] = 0;
}

void execute()
{
    write_trace();

    if (state.rt == IMM_REG_NUM || state.rs == IMM_REG_NUM || state.rd == IMM_REG_NUM)
        state.pc += 2;
    else
        state.pc++;

    // Execute the instuction
    switch (state.opcode)
    {
    case ADD_OP_NUM:
        add();
        break;
    case SUB_OP_NUM:
        sub();
        break;
    case MUL_OP_NUM:
        mul();
        break;
    case AND_OP_NUM:
        and();
        break;
    case OR_OP_NUM:
        or ();
        break;
    case XOR_OP_NUM:
        xor();
        break;
    case SLL_OP_NUM:
        sll();
        break;
    case SRA_OP_NUM:
        sra();
        break;
    case SRL_OP_NUM:
        srl();
        break;
    case BEQ_OP_NUM:
        beq();
        break;
    case BNE_OP_NUM:
        bne();
        break;
    case BLT_OP_NUM:
        blt();
        break;
    case BGT_OP_NUM:
        bgt();
        break;
    case BLE_OP_NUM:
        ble();
        break;
    case BGE_OP_NUM:
        bge();
        break;
    case JAL_OP_NUM:
        jal();
        break;
    case LW_OP_NUM:
        UpdateTimer();
        lw();
        break;
    case SW_OP_NUM:
        UpdateTimer();
        sw();
        break;
    case RETI_OP_NUM:
        reti();
        break;
    case IN_OP_NUM:
        in();
        break;
    case OUT_OP_NUM:
        out();
        break;
    case HALT_OP_NUM:
        halt();
        break;

    default:
        printf("ERROR: opcode %d not recognized\n", state.opcode);
        close_all_files();
        exit(-1);
        break;
    }
}

void write_to_disk()
{
    if (state.ioRegisters[DISK_STATUS] == FREE)
    {
        state.timer_disk = 0;
        if (state.ioRegisters[DISK_CMD] == WRITE_SECTOR)
        {
            state.ioRegisters[DISK_STATUS] = BUSY;
            for (int i = 0; i < SIZE_SECTOR; i++)
                state.disk[state.ioRegisters[DISK_SECTOR]][i] = state.memory[state.ioRegisters[DISK_BUFFER] + i];
        }
        else if (state.ioRegisters[DISK_CMD] == READ_SECTOR)
        {
            state.ioRegisters[DISK_STATUS] = BUSY;
            for (int i = 0; i < SIZE_SECTOR; i++)
                state.memory[state.ioRegisters[DISK_BUFFER] + i] = state.disk[state.ioRegisters[DISK_SECTOR]][i];
        }
    }

    if (state.timer_disk == DISK_CYCLES)
    {
        state.timer_disk = 0;
        state.ioRegisters[DISK_STATUS] = FREE;
        state.ioRegisters[IRQ1STATUS] = TRUE;
        state.ioRegisters[DISK_CMD] = NO_COMMAND;
    }
}

void run_processor()
{
    while (!state.stop_running)
    {
        perform_interrupt();

        fetch();   // IF stage
        decode();  // ID stage
        execute(); // EX stage

        write_to_disk();
    }
}
