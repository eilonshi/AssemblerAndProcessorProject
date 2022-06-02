#include "global.h"

int GetNextIRQ2()
{
	int num = 0;
	if (!feof(irq2in_file) && fscanf_s(irq2in_file, "%d ", &num))
		return num;
}

void UpdateTimer()
{
    state.ioRegisters[CLKS] = (state.ioRegisters[CLKS] + 1) % 0xffffffff;
	
	if (state.ioRegisters[TIMER_ENABLE] == TRUE)
	{
		if (state.ioRegisters[TIMER_CURRENT] == state.ioRegisters[TIMER_MAX])
		{
			state.ioRegisters[IRQ0STATUS] = TRUE;
			state.ioRegisters[TIMER_CURRENT] = 0;
		}
		else
			state.ioRegisters[TIMER_CURRENT]++;
	}

	if (state.ioRegisters[DISK_STATUS] == BUSY)
    {
        state.timer_disk++;
    }
}

void perform_interrupt()
{
	int irq = (state.state.ioRegistersisters[IRQ0ENABLE] && state.ioRegisters[IRQ0STATUS]) || (state.ioRegisters[IRQ1ENABLE] && state.ioRegisters[IRQ1STATUS]) || (state.ioRegisters[IRQ2ENABLE] && state.ioRegisters[IRQ2STATUS]);
	if (irq && !state.isActiveIRQ)
	{
		state.isActiveIRQ = TRUE;
		state.ioRegisters[IRQ_RETURN] = state.pc;  // irqreturn = PC
		state.pc = state.ioRegisters[IRQ_HANDLER]; // PC = irqhandler
	}
	state.ioRegisters[5] = 0; // irq2status = 0
}
