#pragma once

#include "cJSON.h"

typedef enum { CFS_SUCCESS, CFS_EMPTY, CFS_NOENT, CFS_ERROR } FileState;
typedef struct {
	char* name;
	char* cCompiler;
	char* cppCompiler;
	char* generator;
	char* buildTarget;
	char* buildDir;
	char* generateCmd[16];
	char* buildCmd[16];
} Preset;

int cdHere();

int isFlag(const char* arg);
FileState readFile(const char* path, char** buf);

int getJSONString(char** str, const cJSON* json);
int readCmbPresets(const char* path, Preset* preset);
