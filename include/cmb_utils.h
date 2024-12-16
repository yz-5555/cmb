#pragma once

#include "cJSON.h"
#include <stdbool.h>

typedef enum { CFS_SUCCESS, CFS_EMPTY, CFS_NOENT, CFS_ERROR } FileState;
typedef struct {
	char* name;
	char* c_compiler;
	char* cpp_compiler;
	char* generator;
	char* source_dir;
	char* build_target;
	char* build_dir;
	char* run;
	char* generate_cmd[16];
	char* build_cmd[16];
} Preset;

bool cd_here();

bool is_flag(const char* arg);
FileState read_file(const char* path, char** buf);

void get_json_string(char** str, const cJSON* json);
bool is_target(const char* preset_name, const char* target_name);
int get_preset(const char* path, const char* preset_name, Preset* preset);
void free_preset(Preset* preset);

int run_command(const char* format, ...);
