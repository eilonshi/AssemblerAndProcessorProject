#include "global.h"
#include "files.h"

void initialize(const char *argv[])
{
	OpenAllFiles(argv);
	MeminToArr();
	DiskinToArr();
	Irq2inToArr();
}

int main(int argc, char *argv[])
{
	initialize(argv);
	runProcessor();
	summarizeInFiles();
	CloseAllFiles();
}