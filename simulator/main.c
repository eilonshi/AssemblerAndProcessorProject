#include "global.h"
#include "files.h"
#include "processor.h"
#include "state.h"

void initialize(char *argv[]);
void summarize_in_files();

void initialize(char *argv[])
{
	initialize_globals();
	initialize_state();
	open_all_files(argv);

	read_memin();
	read_diskin();
	read_next_irq2();
}

void summarize_in_files()
{
	write_memout();
	write_regout();
	write_cycles();
	write_diskout();
	write_monitor();
	close_all_files();
}

int main(int argc, char *argv[])
{
	initialize(argv);
	run_processor();
	summarize_in_files();
}
