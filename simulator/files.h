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

void OpenAllFiles();
void CloseAllFiles();