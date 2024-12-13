#pragma once

#include <stdio.h>

// file system
int cdHere();
int makeRoot(const char* projectName);
int writeMain(FILE** file);
int writeCMakeLists(FILE** file, const char* projectName);
int writeCmbConfigs(FILE** file);

// configs
typedef enum { CS_SUCCESS, CS_EMPTY, CS_NOENT, CS_ERROR } ConfigsState;
ConfigsState openCmbConfigs(FILE** file, const char* path);
int readCmbConfigs(FILE* file);
