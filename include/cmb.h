#pragma once

// utils
int cmb_setup();
// commands
void cmb_help();
int cmb_new(int argc, char* argv[]);
void cmb_init(int argc, char* argv[]);
void cmb_build(int argc, char* argv[]);
void cmb_clean(int argc, char* argv[]);
void cmb_run(int argc, char* argv[]);
