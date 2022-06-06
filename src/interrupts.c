#include "global.h"
#include "state.h"
#include "files.h"

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

void update_irq2()
{
	if (state.next_irq2 != NO_INTERRUPT && state.ioRegisters[CLKS] > state.next_irq2)
	{
		state.ioRegisters[IRQ2STATUS] = TRUE;
		read_next_irq2();
	}
}

void perform_interrupt()
{
	update_irq2();
	int irq = (state.ioRegisters[IRQ0ENABLE] && state.ioRegisters[IRQ0STATUS]) || (state.ioRegisters[IRQ1ENABLE] && state.ioRegisters[IRQ1STATUS]) || (state.ioRegisters[IRQ2ENABLE] && state.ioRegisters[IRQ2STATUS]);

	if (irq && !state.isActiveIRQ)
	{
		state.isActiveIRQ = TRUE;
		state.ioRegisters[IRQ_RETURN] = state.pc;
		state.pc = state.ioRegisters[IRQ_HANDLER];
		state.ioRegisters[IRQ2STATUS] = FALSE;
	}
}
