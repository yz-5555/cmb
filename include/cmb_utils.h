#pragma once

typedef enum { CFS_SUCCESS, CFS_EMPTY, CFS_NOENT, CFS_ERROR } FileState;

int cdHere();

int isFlag(const char* arg);
FileState readFile(const char* path, char** buf);

int readCmbConfigs(const char* path);
