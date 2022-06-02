#include "global.h"
#include "files.h"

void initialize(const char *argv[])
{
	initialize_globals();
	open_all_files(argv);
	read_memin();
	read_diskin();
	read_irq2in();
}

void summarize_in_files()
{
	WriteToRegout();
	WriteToCycles();
	WriteToDiskout();
	WriteToMonitor();
	close_all_files();
}

int main(int argc, char *argv[])
{
	initialize(argv);
	run_processor();
	summarize_in_files();
}
