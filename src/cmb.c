#include "cmb.h"
#include "cmb_utils.h"
#include <stdio.h>

// commands
int cmb_help()
{
    printf("Usage: cmb <command> [options]\n"
           "`cmb` is a simple CLI tool for project management and build automation using CMake.\n\n"
           "Commands:\n"
           "  cmb init [preset_name]\n"
           "      Generate CMake build files by reading <preset_name> in ./cmb_presets.json.\n"
           "      - If [preset_name] is omitted, the first object in the JSON array\n"
           "        will be selected.\n\n"

           "  cmb build [preset_name]\n"
           "      Build the project using the build files generated by `cmb init`.\n"
           "      The output (e.g., executable files) will be placed in the\n"
           "      designated output directory.\n\n"

           "  cmb run [preset_name]\n"
           "      Run the output generated by `cmb build`.");
    return 0;
}
int cmb_init(int argc, char* argv[])
{
    Preset preset = {
        .name = NULL,
        .c_compiler = NULL,
        .cpp_compiler = NULL,
        .generator = NULL,
        .source_dir = ".",
        .build_target = "Debug",
        .build_dir = "target",
        .run = NULL,
        .generate_cmd_size = 0,
        .build_cmd_size = 0,
        .run_cmd_size = 0
    };

    int result = 0;
    if (argc > CMD_NAME_IDX + 1 && !is_flag(argv[CMD_NAME_IDX + 1])) {
        result = get_preset(argv[CMD_NAME_IDX + 1], &preset);
    } else {
        result = get_preset(NULL, &preset);
    }

    if (result != 0) {
        // printf("cmb_run error: Something went wrong in get_preset.\n");
        return result;
    }
    run_command("cmake -S %s -B %s\\%s -G %s -DCMAKE_BUILD_TYPE=%s -DCMAKE_C_COMPILER=\"%s\" -DCMAKE_CXX_COMPILER=\"%s\"",
        argc, argv,
        preset.generate_cmd_size, preset.generate_cmd,
        preset.source_dir,
        preset.build_dir,
        preset.build_target,
        preset.generator,
        preset.build_target,
        preset.c_compiler,
        preset.cpp_compiler);

    free_preset(&preset);

    // printf("cmb_init success\n");
    return result;
}
int cmb_build(int argc, char* argv[])
{
    Preset preset = {
        .name = NULL,
        .c_compiler = NULL,
        .cpp_compiler = NULL,
        .generator = NULL,
        .source_dir = ".",
        .build_target = "Debug",
        .build_dir = "target",
        .run = NULL,
        .generate_cmd_size = 0,
        .build_cmd_size = 0,
        .run_cmd_size = 0
    };

    int result = 0;
    if (argc > CMD_NAME_IDX + 1 && !is_flag(argv[CMD_NAME_IDX + 1])) {
        result = get_preset(argv[CMD_NAME_IDX + 1], &preset);
    } else {
        result = get_preset(NULL, &preset);
    }

    if (result != 0) {
        // printf("cmb_run error: Something went wrong in get_preset.\n");
        return result;
    }

    run_command("cmake --build %s\\%s",
        argc, argv,
        preset.build_cmd_size, preset.build_cmd,
        preset.build_dir,
        preset.build_target);

    free_preset(&preset);

    // printf("cmb_build success\n");
    return result;
}
int cmb_run(int argc, char* argv[])
{
    Preset preset = {
        .name = NULL,
        .c_compiler = NULL,
        .cpp_compiler = NULL,
        .generator = NULL,
        .source_dir = ".",
        .build_target = "Debug",
        .build_dir = "target",
        .run = NULL,
        .generate_cmd_size = 0,
        .build_cmd_size = 0,
        .run_cmd_size = 0
    };

    int result = 0;
    if (argc > CMD_NAME_IDX + 1 && !is_flag(argv[CMD_NAME_IDX + 1])) {
        result = get_preset(argv[CMD_NAME_IDX + 1], &preset);
    } else {
        result = get_preset(NULL, &preset);
    }

    if (result != 0) {
        // printf("cmb_run error: Something went wrong in get_preset.\n");
        return result;
    }

    run_command("%s\\%s\\bin\\%s",
        argc, argv,
        preset.run_cmd_size, preset.run_cmd,
        preset.build_dir, preset.build_target, preset.run);

    free_preset(&preset);

    // printf("cmb_run success\n");
    return result;
}
