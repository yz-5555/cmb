#pragma once

// utils
int cmbSetup();
// commands
void cmbHelp();
int cmbNew(char* argv[]);
int cmbInit(int argc, char* argv[]);
void cmbBuild(int argc, char* argv[]);
void cmbClean(int argc, char* argv[]);
void cmbRun(int argc, char* argv[]);
