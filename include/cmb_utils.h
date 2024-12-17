#pragma once

#include "cJSON.h"
#include <stdbool.h>

typedef enum { CMB_FS_SUCCESS,
    CMB_FS_EMPTY,
    CMB_FS_NOENT,
    CMB_FS_ERROR } FileState;
typedef enum { CMB_JS_SUCCESS,
    CMB_JS_WARNING,
    CMB_JS_ERROR } JsonState;
typedef struct {
    char* name;
    char* c_compiler;
    char* cpp_compiler;
    char* generator;
    char* source_dir;
    char* build_target;
    char* build_dir;
    char* run;
    char** generate_cmd;
    size_t generate_cmd_size;
    char** build_cmd;
    size_t build_cmd_size;
    char** run_cmd;
    size_t run_cmd_size;
} Preset;

#define CMB_PRESETS_PATH "cmb_presets.json"
#define CMD_NAME_IDX 1

int find_end_of_options(int argc, char* argv[]);

bool is_flag(const char* arg);
FileState read_file(const char* path, char** buf);

JsonState get_json_string(char** str, const cJSON* json);
JsonState get_json_array(char*** arr, size_t* len, const cJSON* json);

bool is_target(const char* preset_name, const char* target_name);
int get_preset(const char* preset_name, Preset* preset);
void free_preset(Preset* preset);

int run_command(const char* format, int argc, char* argv[], size_t cmd_len, char* cmd[], ...);
