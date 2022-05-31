#include "global.h"

int GetNextIRQ2()
{
	int num = 0;
	if (!feof(irq2in_file) && fscanf_s(irq2in_file, "%d ", &num))
		return num;
}

void UpdateTimer()
{
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
}

void UpdateAndCheckDisk()
{
	int i = 0;
	if (state.ioRegisters[17] == 0) // diskstatus=0 (free to receive new command)
	{
		timer_disk = 0;
		if (state.ioRegisters[14] == 2) // diskcmd=2 (write sector)
		{
			state.ioRegisters[17] = 1;	 // diskstatus=1
			for (; i < SIZE_SECTOR; i++) // MemDisk[disksector]=Memory[diskbuffer]
				MemDisk[state.ioRegisters[15]][i] = Mem[state.ioRegisters[16] + i];
		}
		else if (state.ioRegisters[14] == 1) // diskcmd=1 (read sector)
		{
			state.ioRegisters[17] = 1;	 // diskstatus=1
			for (; i < SIZE_SECTOR; i++) // Memory[diskbuffer]=MemDisk[disksector]
				Mem[state.ioRegisters[16] + i] = MemDisk[state.ioRegisters[15]][i];
		}
	}
	else if (state.ioRegisters[17] == 1) // diskstatus=1 (busy handling a read/write commad)
	{
		timer_disk++;
	}
	if (timer_disk == DISK_CYCLES) // 1024 cycles
	{
		timer_disk = 0;
		state.ioRegisters[17] = 0;
		state.ioRegisters[4] = 1; // irqstatus1=1
		state.ioRegisters[14] = 0;
	}
}

void PerformInterrupt()
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
