// Includes:
#include "ops.h"
#include "global.h"

// Operation implementations:
void add()
{
    state.registers[state.rd] = state.registers[state.rd] + state.registers[state.rt];
}
void sub()
{
    state.registers[state.rd] = state.registers[state.rd] - state.registers[state.rt];
}
void mul()
{
    state.registers[state.rd] = state.registers[state.rd] * state.registers[state.rt];
}
void and ()
{
    state.registers[state.rd] = state.registers[state.rd] & state.registers[state.rt];
}
void or ()
{
    state.registers[state.rd] = state.registers[state.rd] | state.registers[state.rt];
}
void xor () { state.registers[state.rd] = state.registers[state.rd] ^ state.registers[state.rt]; } void sll() { state.registers[state.rd] = state.registers[state.rd] << state.registers[state.rt]; }
void sra()
{
    state.registers[state.rd] = state.registers[state.rd] >> state.registers[state.rt];
}
void srl()
{
} // Todo
void beq()
{
    if (state.registers[state.rd] == state.registers[state.rt])
        state.pc = state.registers[state.rd];
}
void bne()
{
    if (state.registers[state.rd] != state.registers[state.rt])
        state.pc = state.registers[state.rd];
}
void blt()
{
    if (state.registers[state.rd] < state.registers[state.rt])
        state.pc = state.registers[state.rd];
}
void bgt()
{
    if (state.registers[state.rd] > state.registers[state.rt])
        state.pc = state.registers[state.rd];
}
void ble()
{
    if (state.registers[state.rd] <= state.registers[state.rt])
        state.pc = state.registers[state.rd];
}
void bge()
{
    if (state.registers[state.rd] >= state.registers[state.rt])
        state.pc = state.registers[state.rd];
}
void jal()
{
    state.registers[state.rd] = state.pc + 1;
    state.pc = state.registers[state.rd];
}
void lw()
{
    state.registers[state.rd] = state.memory[state.registers[state.rd] + state.registers[state.rt]];
}
void sw()
{
    state.memory[state.registers[state.rd] + state.registers[state.rt]] = state.registers[state.rd];
}
void reti()
{
    state.pc = state.ioRegisters[7];
}
void in()
{
    state.registers[state.rd] = state.ioRegisters[state.registers[state.rd] + state.registers[state.rt]];
}
void out()
{
    state.ioRegisters[state.registers[state.rd] + state.registers[state.rt]] = state.registers[state.rd];
}

// Operation parsing:
int GetRM()
{
    return (state.instruction >> 24) & 0xF;
}
int GetRT()
{
    state.rt = (state.instruction >> 28) & 0xF;
}
int GetRS()
{
    state.rs = (state.instruction >> 32) & 0xF;
}
int GetRD()
{
    state.rd = (state.instruction >> 36) & 0xF;
}
int GetOpcode()
{
    state.opcode = (state.instruction >> 40) & 0xFF;
}