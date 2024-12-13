#include "cmb_utils.h"

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

#include "cJson.h"

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
int makeRoot(const char* projectName)
{
    char currentDir[MAX_PATH];
    
	if (CURRENT_DIR(currentDir) == 0) {
        fprintf(stderr, "cmb: Failed to get current directory.\n");
        return 1;
    }
    if (MKDIR(projectName) == -1 && errno != EEXIST) {
        perror("cmb: Failed to create project root, ");
        return 1;
    }
    if (CHDIR(projectName) == 1) {
        fprintf(stderr, "cmb: Failed to cd to current directory.\n");
        return 1;
    }
    if (MKDIR("src") == -1 && errno != EEXIST) {
        perror("cmb: Failed to create src, ");
        return 1;
    }
    return 0;
}
int writeMain(FILE** file)
{
    if (fopen_s(file, "src/main.c", "w") != 0) {
        perror("cmb: Failed to create main.c.");
        return 1;
    }

    fprintf(*file, "#include <stdio.h>\n");
    fprintf(*file, "int main(void)\n{\n");
    fprintf(*file, "    printf(\"Hello, World!\");\n\n");
    fprintf(*file, "    return 0;\n}");

    fclose(*file);

    return 0;
}
int writeCMakeLists(FILE** file, const char* projectName)
{
    if (fopen_s(file, "CMakeLists.txt", "w") != 0) {
        perror("cmb: Failed to create CMakeLists.txt, ");
        return 1;
    }

    fprintf(*file, "cmake_minimum_required(VERSION 3.16)\n");
    fprintf(*file, "project(%s)", projectName);
    
	fclose(*file);

    return 0;
}
int writeCmbConfigs(FILE** file)
{
    if (fopen_s(file, "cmb_configs.json", "w") != 0) {
        perror("cmb: Failed to create cmb_configs.json, ");
        return 1;
    }

    fprintf(*file, "[\n");
    fprintf(*file, "    {}\n");
    fprintf(*file, "]");

    fclose(*file);

    return 0;
}
ConfigsState openCmbConfigs(FILE** file, const char* path)
{
	int result = fopen_s(file, path, "r");
	
	if (result == ENOENT)
		return CS_NOENT;

	if (result != 0 && result != ENOENT) {
		perror("cmb: Failed to read cmb_configs.json, ");
		return CS_ERROR;
	}
	
	char buf[256] = { '\0' };
	fread(buf, sizeof(char), sizeof(buf), *file);

	if (buf[0] == '\0')
		return CS_EMPTY;

	return CS_SUCCESS;
}
int readCmbConfigs(FILE* file)
{
	
	return 0;
}
