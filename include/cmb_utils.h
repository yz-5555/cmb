#pragma once

#include <stdio.h>

int cd_here();
int make_root(const char* project_name);

int make_directories();

int write_main(FILE* file);
int write_cmake_lists(FILE* file, const char* project_name);
int write_cmb_config(FILE* file);
