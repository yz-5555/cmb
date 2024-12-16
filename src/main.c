#include "cmb.h"
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
        return cmb_help();

    if (strcmp(argv[1], "-h") == 0
        || strcmp(argv[1], "--help") == 0
        || strcmp(argv[1], "help") == 0) {
        return cmb_help();
    } else if (strcmp(argv[1], "init") == 0) {
		return cmb_init(argc, argv);
	} else if (strcmp(argv[1], "build") == 0) {
		return cmb_build(argc, argv);
	} else if (strcmp(argv[1], "run") == 0) {
		return cmb_run(argc, argv);
	} else {
		return cmb_help();
	}
}
