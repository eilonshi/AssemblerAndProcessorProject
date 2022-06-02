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

void close_all_files()
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
    fprintf(leds_file, "%d %08x\n", state.ioRegisters[CLKS], state.ioRegisters[LEDS]);
}

void WriteToDisplay7SegFile()
{
    fprintf(display7seg_file, "%d %08x\n", state.ioRegisters[CLKS], state.ioRegisters[DISPLAY7SEG]);
}

void WritePixelToMonitor()
{
    int x = state.ioRegisters[MONITOR_ADDR] & 0X00FF;
    int y = (state.ioRegisters[MONITOR_ADDR] >> 8) & 0X00FF;
    state.monitor[x][y] = state.ioRegisters[MONITOR_DATA];
}

void WriteToHwregtraceFile(char *operation_read_or_write)
{
    fprintf(hwregtrace_file, "%d %s %s %08x\n", state.ioRegisters[CLKS], operation_read_or_write, IO_REGISTER_NAMES[state.registers[state.rs] + state.registers[state.rt]], state.ioRegisters[state.registers[state.rs] + state.registers[state.rt]]);
}

void WriteToTrace()
{
    fprintf(trace_file, "%03X %012llX ", state.pc, state.instruction);
    for (int i = 0; i < NUM_REGISTERS; i++)
        fprintf(trace_file, "%08x ", state.registers[i]);
    fprintf(trace_file, "\n");
}

void WriteToRegout()
{
    for (int i = PRINT_FROM_REGISTER_NUM; i < NUM_REGISTERS; i++)
        fprintf(regout_file, "%08X\n", state.registers[i]);
}

void WriteToCycles()
{
    fprintf(cycles_file, "%d\n", state.ioRegisters[8]);
}

void WriteToDiskout()
{
    // Calc max addresses of non zero data
    int max_i;
    int max_j;
    int flag = FALSE;
    for (max_i = NUM_SECTORS - 1; max_i >= 0; max_i--)
    {
        for (max_j = SIZE_SECTOR - 1; max_j >= 0; max_j--)
        {
            if (state.disk[max_i][max_j])
            {
                flag = TRUE;
                break;
            }
        }
        if (flag)
            break;
    }

    // Write to diskout_file
    for (int i = 0; i <= max_i; i++)
    {
        for (int j = 0; j <= max_j; j++)
        {
            fprintf(diskout_file, "%012llX\n", state.disk[i][j]);
        }
    }
}

void WriteToMonitor()
{
    unsigned char monitorYuv[MONITOR_WIDTH * MONITOR_HEIGHT] = {0};
    int pixel_num = 0;

    for (int row = 0; row < MONITOR_HEIGHT; row++)
    {
        for (int col = 0; col < MONITOR_WIDTH; col++)
        {
            fprintf(monitor_file, "%02X\n", state.monitor[col][row]);
            monitorYuv[pixel_num] = state.monitor[col][row];
            pixel_num++;
        }
    }
    fwrite(monitorYuv, 1, MONITOR_WIDTH * MONITOR_HEIGHT, monitor_yuv_file);
}
