#include "global.h"

const char *IO_REGISTER_NAMES[NUM_IO_REGISTERS] = {"irq0enable", "irq1enable", "irq2enable", "irq0status", "irq1status", "irq2status", "irqhandler", "irqreturn", "clks", "leds", "display7seg", "timerenable", "timercurrent", "timermax", "diskcmd", "disksector", "diskbuffer", "diskstatus", "reserved", "reserved", "monitoraddr", "monitordata", "monitorcmd"};

void initialize_globals()
{
}
