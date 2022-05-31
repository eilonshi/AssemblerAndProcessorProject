#include <stdio.h>
#include "global.h"

FILE *memin_file = NULL;
FILE *diskin_file = NULL;
FILE *irq2in_file = NULL;
FILE *memout_file = NULL;
FILE *regout_file = NULL;
FILE *trace_file = NULL;
FILE *hwregtrace_file = NULL;
FILE *cycles_file = NULL;
FILE *leds_file = NULL;
FILE *display7seg_file = NULL;
FILE *diskout_file = NULL;
FILE *monitor_file = NULL;
FILE *monitor_yuv_file = NULL;

void OpenAllFiles(const char *argv[])
{
    memin_file = OpenFile(argv[1], "r");
    diskin_file = OpenFile(argv[2], "r");
    irq2in_file = OpenFile(argv[3], "r");
    memout_file = OpenFile(argv[4], "w");
    regout_file = OpenFile(argv[5], "w");
    trace_file = OpenFile(argv[6], "w");
    hwregtrace_file = OpenFile(argv[7], "w");
    cycles_file = OpenFile(argv[8], "w");
    leds_file = OpenFile(argv[9], "w");
    display7seg_file = OpenFile(argv[10], "w");
    diskout_file = OpenFile(argv[11], "w");
    monitor_file = OpenFile(argv[12], "w");
    monitor_yuv_file = OpenFile(argv[13], "wb");
}

void CloseAllFiles()
{
    if (memin_file != NULL)
        fclose(memin_file);
    if (diskin_file != NULL)
        fclose(diskin_file);
    if (irq2in_file != NULL)
        fclose(irq2in_file);
    if (memout_file != NULL)
        fclose(memout_file);
    if (regout_file != NULL)
        fclose(regout_file);
    if (trace_file != NULL)
        fclose(trace_file);
    if (hwregtrace_file != NULL)
        fclose(hwregtrace_file);
    if (cycles_file != NULL)
        fclose(cycles_file);
    if (leds_file != NULL)
        fclose(leds_file);
    if (display7seg_file != NULL)
        fclose(display7seg_file);
    if (diskout_file != NULL)
        fclose(diskout_file);
    if (monitor_file != NULL)
        fclose(monitor_file);
    if (monitor_yuv_file != NULL)
        fclose(monitor_yuv_file);
}

void WriteToLedsFile()
{
    fprintf(leds_file, "%d %08x\n", state.ioRegisters[8], state.ioRegisters[9]);
}

void WriteToDisplay7SegFile()
{
    fprintf(display7seg_file, "%d %08x\n", state.ioRegisters[8], state.ioRegisters[10]);
}

void WritePixelToMonitor()
{
    int x = state.ioRegisters[MONITOR_ADDR] & 0X00FF;
    int y = (state.ioRegisters[MONITOR_ADDR] >> 8) & 0X00FF;
    Monitor[x][y] = state.ioRegisters[MONITOR_DATA];
    // TODO: write to monitor file
}

void WriteToHwregtraceFile()
{
    fprintf(hwregtrace_file, "%d WRITE %s %08x\n", state.ioRegisters[8], IO_reg_names[Regsters[rs] + Regsters[rt]], state.ioRegisters[Regsters[rs] + Regsters[rt]]);
}
