#include "cmb.h"
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cmb_help();
        return 1;
    }

	if (cmb_setup() == 1)
		return 1;

    if (strcmp(argv[1], "-h") == 0
        || strcmp(argv[1], "--help") == 0
        || strcmp(argv[1], "help") == 0) {
        cmb_help();
    } else if (strcmp(argv[1], "new") == 0) {
        return cmb_new(argc, argv);
    } else if (strcmp(argv[1], "init") == 0) {
		cmb_init(argc, argv);
	} else if (strcmp(argv[1], "build") == 0) {
		cmb_build(argc, argv);
	} else if (strcmp(argv[1], "run") == 0) {
		cmb_run(argc, argv);
	} else {
		cmb_help();
	}
    return 0;
}
