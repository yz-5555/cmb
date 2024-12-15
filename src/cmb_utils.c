#include "cmb_utils.h"
#include "cJSON.h"
#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(x) _mkdir(x)
#define CHDIR(x) _chdir(x)
#include <Windows.h>
#define CURRENT_DIR(dir) GetCurrentDirectory(MAX_PATH, dir)
#else
#include <sys/stat.h>
#define MKDIR(x) mkdir(x, 0755)
#define CHDIR(x) chdir(x)
#include <limits.h>
#include <unistd.h>
#define CURRENT_DIR(dir) getcwd(dir, sizeof(dir))
#define MAX_PATH PATH_MAX
#endif

#include "cJSON.h"

int cdHere()
{
    char currentDir[MAX_PATH];

    if (CURRENT_DIR(currentDir) == 0) {
        fprintf(stderr, "cmb: Failed to get current directory.\n");
        return 1;
    }
    if (CHDIR(currentDir) == 1) {
        fprintf(stderr, "cmb: Failed to cd to current directory.\n");
        return 1;
    }
    return 0;
}
inline int isFlag(const char* arg)
{
    return arg[0] == '-';
}
FileState readFile(const char* path, char** buf)
{
	FILE* file = NULL;
	int result = fopen_s(&file, path, "r");

	if (result == ENOENT) {
		fprintf(stderr, "cmb_utils: Failed to find the file.");
		return CFS_NOENT;
	}

	if (result != 0 && result != ENOENT) {
		perror("cmb_utils: Failed to read the file, ");
		return CFS_ERROR;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (size == 0) {
		fprintf(stderr, "cmb_utils: The file is empty.\n");
		return CFS_EMPTY;
	}

	*buf = (char *)malloc(size + 1);
	if (*buf == NULL) {
		perror("cmb_utils: Failed to allocate memory, ");
		return CFS_ERROR;
	}

	fread(*buf, sizeof(char), sizeof(*buf), file);
	(*buf)[size] = '\0';

	fclose(file);

	return CFS_SUCCESS;
}
int readCmbConfigs(const char* path)
{
	char* buf = NULL;
	FileState state = readFile(path, &buf);
	if (state != CFS_SUCCESS) {
		fprintf(stderr, "cmb_utils: Failed to read cmb_configs.json.\n");
		return 1;
	}

	cJSON* json = cJSON_Parse(buf);
	if (json == NULL) {
		const char* err = cJSON_GetErrorPtr();
		if (err != NULL) {
			fprintf(stderr, "cmb_utils: Failed to parse cmb_configs.json: %s\n", err);
			cJSON_Delete(json);
			return 1;
		}
	}
	free(buf);
	
	if (!cJSON_IsArray(json)) {
		fprintf(stderr, "cmb_utils: Expected a JSON array in cmb_configs.json.\n");
		cJSON_Delete(json);
		return 1;
	}

	int arraySize = cJSON_GetArraySize(json);
	if (arraySize == 0) {
		fprintf(stderr, "cmb_utils: cmb_configs.json is empty.\n");
		cJSON_Delete(json);
		return 1;
	}
	
	cJSON* preset;
	cJSON_ArrayForEach(preset, json)
	{
		// TODO: parse the damn configs
	}
	
    return 0;
}
