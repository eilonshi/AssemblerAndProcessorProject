#include <stdio.h>

extern FILE *memin_file;
extern FILE *diskin_file;
extern FILE *irq2in_file;
extern FILE *memout_file;
extern FILE *regout_file;
extern FILE *trace_file;
extern FILE *hwregtrace_file;
extern FILE *cycles_file;
extern FILE *leds_file;
extern FILE *display7seg_file;
extern FILE *diskout_file;
extern FILE *monitor_file;
extern FILE *monitor_yuv_file;

/* Opens all the files for the simulator */
void open_all_files(char *argv[]);
/* Closes all the files for the simulator */
void close_all_files();

/* Reads the memory from the memin.txt file to an array */
void read_memin();
/* Reads the disk from the diskin.txt file to an array */
void read_diskin();
/* Reads the next clock cycle number for irq2 interrupt */
void read_next_irq2();

/* Writes the memory at the end of the simulation to memout.txt file */
void write_memout();
/* Writes the leds at the end of the simulation to leds.txt file */
void write_leds();
/* Writes the display7seg at the end of the simulation to display7seg.txt file */
void write_display7seg();
/* Writes the hardware trace during the simulation to hwregtrace.txt file */
void write_hwregtrace();
/* Writes the registers trace during the simulation to trace.txt file */
void write_trace();
/* Writes the registers at the end of the simulation to regout.txt file */
void write_regout();
/* Writes the number of cycles of the simulation to cycles.txt file */
void write_cycles();
/* Writes the disk at the end of the simulation to diskout.txt file */
void write_diskout();
/* Writes the monitor at the end of the simulation to monitor.txt and monitor.yuv files */
void write_monitor();
