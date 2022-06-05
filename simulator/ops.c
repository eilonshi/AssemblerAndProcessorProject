// Includes:
#include "ops.h"
#include "global.h"
#include "files.h"
#include "state.h"

// Operation implementations:

// Addition
void add()
{
    state.registers[state.rd] = state.registers[state.rs] + state.registers[state.rt];
}

// Subtruction
void sub()
{
    state.registers[state.rd] = state.registers[state.rs] - state.registers[state.rt];
}

// Multiplication
void mul()
{
    state.registers[state.rd] = state.registers[state.rs] * state.registers[state.rt];
}

// AND bitwise operator
void and ()
{
    state.registers[state.rd] = state.registers[state.rs] & state.registers[state.rt];
}

// OR bitwise operator
void or ()
{
    state.registers[state.rd] = state.registers[state.rs] | state.registers[state.rt];
}

// XOR bitwise operator
void xor () {
    state.registers[state.rd] = state.registers[state.rs] ^ state.registers[state.rt];
}

    // Shift left logical
    void sll()
{
    state.registers[state.rd] = state.registers[state.rs] << state.registers[state.rt];
}

// Shift right arighmetic
void sra()
{
    state.registers[state.rd] = state.registers[state.rs] >> state.registers[state.rt];
}

// Shift right logical
void srl()
{
    state.registers[state.rd] = (int)((unsigned int)state.registers[state.rs] >> state.registers[state.rt]);
}

// Branch on equal
void beq()
{
    if (state.registers[state.rs] == state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on not equal
void bne()
{
    if (state.registers[state.rs] != state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on less than
void blt()
{
    if (state.registers[state.rs] < state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on greater than
void bgt()
{
    if (state.registers[state.rs] > state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on less than or equal
void ble()
{
    if (state.registers[state.rs] <= state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Branch on greater than or equal
void bge()
{
    if (state.registers[state.rs] >= state.registers[state.rt])
        state.pc = state.registers[state.rd];
}

// Jump and link
void jal()
{
    state.registers[state.rd] = state.pc;
    state.pc = state.registers[state.rs];
}

// Load word
void lw()
{
    state.registers[state.rd] = state.memory[state.registers[state.rs] + state.registers[state.rt]];
}

// Store word
void sw()
{
    state.memory[state.registers[state.rs] + state.registers[state.rt]] = state.registers[state.rd];
}

// Return from interrupt
void reti()
{
    state.pc = state.ioRegisters[IRQ_RETURN];
    state.isActiveIRQ = FALSE;
    printf("%d %03X returning from interrupt\n", state.ioRegisters[CLKS], state.pc);
}

// In - read from I/O
void in()
{
    state.registers[state.rd] = state.ioRegisters[state.registers[state.rs] + state.registers[state.rt]];
    write_hwregtrace("READ");
}

// Out - write to I/O
void out()
{
    int write_to = state.registers[state.rs] + state.registers[state.rt];
    state.ioRegisters[write_to] = state.registers[state.rd];
    // printf("writing %d to io[%d]\n", state.registers[state.rd], write_to);

    switch (write_to)
    {
    case LEDS:
        write_leds();
        break;
    case DISPLAY7SEG:
        write_display7seg();
        break;
    case MONITOR_CMD:
        if (state.ioRegisters[MONITOR_CMD])
            update_monitor();
        break;
    default:
        break;
    }

    write_hwregtrace("WRITE");
}

// Halt - stop running
void halt()
{
    state.stop_running = TRUE;
}

int sign_extention_from_20_bit_to_32_bit(int num)
{
    int extended_num = num;
    extended_num = extended_num << 12;
    extended_num = extended_num >> 12;

    return extended_num;
}

// Operation parsing:
int get_rt()
{
    return state.instruction & 0xF;
}
int get_rs()
{
    return (state.instruction >> 4) & 0xF;
}
int get_rd()
{
    return (state.instruction >> 8) & 0xF;
}
int get_opcode()
{
    return (state.instruction >> 12) & 0xFF;
}
int get_imm()
{
    int imm = state.imm & 0xFFFFF;
    int extended_imm = sign_extention_from_20_bit_to_32_bit(imm);
    return extended_imm;
}
