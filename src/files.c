#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "global.h"
#include "state.h"

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

FILE *open_file(char f_name[], char f_type[])
{
    FILE *Output_file = fopen(f_name, f_type);
    if (Output_file == NULL)
    {
        printf("ERROR: can't open %s file\n", f_name);
        close_all_files();
        exit(-1);
    }

    return Output_file;
}

void open_all_files(char *argv[])
{
    memin_file = open_file(argv[1], "r");
    diskin_file = open_file(argv[2], "r");
    irq2in_file = open_file(argv[3], "r");

    memout_file = open_file(argv[4], "w");
    regout_file = open_file(argv[5], "w");
    trace_file = open_file(argv[6], "w");
    hwregtrace_file = open_file(argv[7], "w");
    cycles_file = open_file(argv[8], "w");
    leds_file = open_file(argv[9], "w");
    display7seg_file = open_file(argv[10], "w");
    diskout_file = open_file(argv[11], "w");
    monitor_file = open_file(argv[12], "w");
    monitor_yuv_file = open_file(argv[13], "wb");
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

void read_memin()
{
    char line[INSTRUCTION_LENGTH_HEX];
    int i = 0;
    while (!feof(memin_file) && fgets(line, INSTRUCTION_LENGTH_HEX, memin_file))
    {
        if (strcmp(line, "\n"))
        {
            state.memory[i] = (int)strtol(line, NULL, LONG_BYTES_NUM);
            i++;
        }
    }
}

void read_diskin()
{
    char line[INSTRUCTION_LENGTH_HEX];
    int i = 0, j = 0;
    while (!feof(diskin_file) && fgets(line, INSTRUCTION_LENGTH_HEX, diskin_file))
    {
        if (strcmp(line, "\n"))
        {
            if (j == SIZE_SECTOR)
            {
                j = 0;
                i++;
            }
            state.disk[i][j] = (int)strtol(line, NULL, LONG_BYTES_NUM);
            j++;
        }
    }
}

void read_next_irq2()
{
    int num = 0;
    if (!feof(irq2in_file))
    {
        if (fscanf(irq2in_file, "%d ", &num))
            state.next_irq2 = num;
    }
    else
        state.next_irq2 = NO_INTERRUPT;
}

void write_memout()
{
    // Find the maximum address of non-zero memory
    int max_non_zero_address = MEMORY_SIZE - 1;
    for (; max_non_zero_address >= 0; max_non_zero_address--)
        if (state.memory[max_non_zero_address] != 0)
            break;

    for (int i = 0; i <= max_non_zero_address; i++)
        fprintf(memout_file, "%05X\n", state.memory[i]);
}

void write_leds()
{
    fprintf(leds_file, "%d %08X\n", state.ioRegisters[CLKS] - 1, state.ioRegisters[LEDS]);
}

void write_display7seg()
{
    fprintf(display7seg_file, "%d %08X\n", state.ioRegisters[CLKS] - 1, state.ioRegisters[DISPLAY7SEG]);
}

void write_hwregtrace(char operation_read_or_write[])
{
    fprintf(hwregtrace_file, "%d %s %s %08X\n", state.ioRegisters[CLKS] - 1, operation_read_or_write, IO_REGISTER_NAMES[state.registers[state.rs] + state.registers[state.rt]], state.ioRegisters[state.registers[state.rs] + state.registers[state.rt]]);
}

void write_trace()
{
    fprintf(trace_file, "%03X %05X ", state.pc, state.instruction);
    for (int i = 0; i < NUM_REGISTERS; i++)
    {
        fprintf(trace_file, "%08X", state.registers[i]);
        if (i + 1 < NUM_REGISTERS)
            fprintf(trace_file, " ");
    }
    fprintf(trace_file, "\n");
}

void write_regout()
{
    for (int i = PRINT_FROM_REGISTER_NUM; i < NUM_REGISTERS; i++)
        fprintf(regout_file, "%08X\n", state.registers[i]);
}

void write_cycles()
{
    fprintf(cycles_file, "%d\n", state.ioRegisters[8]);
}

void write_diskout()
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
            fprintf(diskout_file, "%05X\n", state.disk[i][j]);
        }
    }
}

void write_monitor()
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
