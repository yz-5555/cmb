#include "cmb.h"
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cmbHelp();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0
        || strcmp(argv[1], "--help") == 0
        || strcmp(argv[1], "help") == 0) {
        cmbHelp();
    } else if (strcmp(argv[1], "init") == 0) {
		return cmbInit(argc, argv);
	} else if (strcmp(argv[1], "build") == 0) {
		cmbBuild(argc, argv);
	} else if (strcmp(argv[1], "run") == 0) {
		cmbRun(argc, argv);
	} else {
		cmbHelp();
	}
    return 0;
}
