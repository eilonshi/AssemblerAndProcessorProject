// Includes:
#include "ops.h"
#include "global.h"

// Operation implementations:

// Addition
void add()
{
    state.registers[state.rd] = state.registers[state.rd] + state.registers[state.rt];
}

// Subtruction
void sub()
{
    state.registers[state.rd] = state.registers[state.rd] - state.registers[state.rt];
}

// Multiplication
void mul()
{
    state.registers[state.rd] = state.registers[state.rd] * state.registers[state.rt];
}

// AND bitwise operator
void and ()
{
    state.registers[state.rd] = state.registers[state.rd] & state.registers[state.rt];
}

// OR bitwise operator
void or ()
{
    state.registers[state.rd] = state.registers[state.rd] | state.registers[state.rt];
}

// XOR bitwise operator
void xor () {
    state.registers[state.rd] = state.registers[state.rd] ^ state.registers[state.rt];
}

    // Shift left logical
    void sll()
{
    state.registers[state.rd] = state.registers[state.rd] << state.registers[state.rt];
}

// Shift right arighmetic
void sra()
{
    state.registers[state.rd] = state.registers[state.rd] >> state.registers[state.rt];
}

// Shift right logical
void srl()
{
    state.registers[state.rd] = (int)((unsigned int)state.registers[state.rd] >> state.registers[state.rt]);
}

// Branch on equal
void beq()
{
    if (state.registers[state.rd] == state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on not equal
void bne()
{
    if (state.registers[state.rd] != state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on less than
void blt()
{
    if (state.registers[state.rd] < state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on greater than
void bgt()
{
    if (state.registers[state.rd] > state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on less than or equal
void ble()
{
    if (state.registers[state.rd] <= state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on greater than or equal
void bge()
{
    if (state.registers[state.rd] >= state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Jump and link
void jal()
{
    state.registers[state.rd] = state.pc + 1;
    state.pc = state.registers[state.rs];
}

// Load word
void lw()
{
    state.registers[state.rd] = state.memory[state.registers[state.rd] + state.registers[state.rt]];
}

// Store word
void sw()
{
    state.memory[state.registers[state.rd] + state.registers[state.rt]] = state.registers[state.rd];
}

// Return from interrupt
void reti()
{
    state.pc = state.ioRegisters[IRQ_RETURN];
    state.isActiveIRQ = FALSE;
}

// In - read from I/O
void in()
{
    state.registers[state.rd] = state.ioRegisters[state.registers[state.rs] + state.registers[state.rt]];
}

// Out - write to I/O
void out()
{
    int write_to = state.registers[state.rs] + state.registers[state.rt];
    state.ioRegisters[write_to] = state.registers[state.rd];

    switch (write_to)
    {
    case LEDS:
        WriteToLedsFile();
        break;
    case DISPLAY7SEG:
        WriteToDisplay7SegFile();
        break;
    case MONITOR_CMD:
        if (state.ioRegisters[MONITOR_CMD])
            WritePixelToMonitor();
        break;
    default:
        break;
    }

    WriteToHwregtraceFile();
}

// Operation parsing:
int get_rt()
{
    state.rt = state.instruction & 0xF;
}
int get_rs()
{
    state.rs = (state.instruction >> 4) & 0xF;
}
int get_rd()
{
    state.rd = (state.instruction >> 8) & 0xF;
}
int get_opcode()
{
    state.opcode = (state.instruction >> 12) & 0xFF;
}
int GetImm()
{
    return state.instruction & 0xFFFFF;
}