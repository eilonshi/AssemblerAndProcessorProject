// Includes
#include "ops.h"
#include "global.h"
#include "files.h"

// Consts:
#define TRUE 1
#define FALSE 0

void fetch()
{
    // Update Pipeline Register
    state.instruction = state.memory[state.pc];
    state.pc = state.pc + 1;
}

void decode()
{
    state.imm = GetImm();
    state.rm = GetRM();
    state.rt = GetRT();
    state.rs = GetRS();
    state.rd = GetRD();
    state.opcode = GetOpcode();
}

void execute()
{
    // Do the operation
    switch (state.opcode)
    {
    case ADD_OP_NUM:
        add();
        break;
    case SUB_OP_NUM:
        sub();
        break;
    case MUL_OP_NUM:
        mac();
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
        lw();
        break;
    case SW_OP_NUM:
        sw();
        break;
    case RETI_OP_NUM:
        reti();
        break;
    case IN_OP_NUM:
        in();
        // Write to hwregtrace_file (read)
        fprintf(hwregtrace_file, "%d READ %s %08x\n", IOReg[8], IO_reg_names[Regsters[rs] + Regsters[rt]], IOReg[Regsters[rs] + Regsters[rt]]);
        break;
    case OUT_OP_NUM:
        out();
        if (Regsters[rs] + Regsters[rt] == 9) // The inst changes the value of the leds reg
        {
            // Write to leds_file
            fprintf(leds_file, "%d %08x\n", IOReg[8], IOReg[9]);
        }
        if (Regsters[rs] + Regsters[rt] == 10) // The inst changes the value of the display7seg reg
        {
            // Write to display7seg_file
            fprintf(display7seg_file, "%d %08x\n", IOReg[8], IOReg[10]);
        }
        // if ((Regsters[rs] + Regsters[rt] == 22) & (IOReg[Regsters[rs] + Regsters[rt]] == 1))  // The inst turn on the monitorcmd reg
        if (Regsters[rs] + Regsters[rt] == 22 && IOReg[22] == 1)
        {
            int x = IOReg[20] & 0X00FF;
            int y = (IOReg[20] >> 8) & 0X00FF;
            Monitor[x][y] = IOReg[21]; // Put the monitordata in the monitor matrix
            IOReg[22] = 0;
        }
        // Write to hwregtrace_file (write)
        fprintf(hwregtrace_file, "%d WRITE %s %08x\n", IOReg[8], IO_reg_names[Regsters[rs] + Regsters[rt]], IOReg[Regsters[rs] + Regsters[rt]]);
        break;
    case HALT_OP_NUM:
        break;
    
    default:
        // Error.
        fprintf("ERROR: Instruction not recognized:" );
        printInstruction(state.IDEX.instr);
        exit(1);
        break;
    }
    UpdateAndCheckDisk();
    PerformInterupt();
    IOReg[8] = (IOReg[8] + 1) % 0xffffffff;
}

void memory()
{
}

void writeBack()
{
}


void runProcessor()
{
    while (TRUE)
    {
        fetch();     // IF stage
        decode();    // ID stage
        execute();   // EX stage
        memory();    // MEM stage
        writeBack(); // WB stage
    }
}
