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

int cd_here()
{
    char current_dir[MAX_PATH];
    if (CURRENT_DIR(current_dir) == 0) {
        fprintf(stderr, "cmb: Failed to get current directory.\n");
        return 1;
    }
    if (CHDIR(current_dir) == 1) {
        fprintf(stderr, "cmb: Failed to cd to current directory.\n");
        return 1;
    }
    return 0;
}
int make_root(const char* project_name)
{
    char current_dir[MAX_PATH];
    if (CURRENT_DIR(current_dir) == 0) {
        fprintf(stderr, "cmb: Failed to get current directory.\n");
        return 1;
    }
	if (MKDIR(project_name) == -1 && errno != EEXIST) {
		perror("Failed to create project root.");
		return 1;
	}
	
	size_t current_dir_len = strlen(current_dir);
	size_t project_name_len = strlen(project_name);

	if (current_dir_len + 1 + project_name_len > MAX_PATH) {
		fprintf(stderr, "cmb: Path of the project is too long.\n");
		return 1;
	}

	if (current_dir_len > 0 && current_dir[current_dir_len - 1] != '/') {
		snprintf(current_dir + current_dir_len, MAX_PATH - current_dir_len, "/%s", project_name);
	} else {
		snprintf(current_dir + current_dir_len, MAX_PATH - current_dir_len, "%s", project_name);
	}

    if (CHDIR(current_dir) == 1) {
        fprintf(stderr, "cmb: Failed to cd to current directory.\n");
        return 1;
    }
    return 0;
}
int make_directories()
{
	if (MKDIR("src") == -1 && errno != EEXIST) {
		perror("Failed to create src/.");
		return 1;
	}
	if (MKDIR("target") == -1 && errno != EEXIST) {
		perror("Failed to create target/.");
		return 1;
	}
	return 0;
}
int write_main(FILE* file)
{
    if (fopen_s(&file, "src/main.c", "w") != 0) {
        perror("Failed to create main.c.");
        return 1;
    }

    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "int main(void)\n{\n");
    fprintf(file, "    printf(\"Hello, World!\");\n\n");
    fprintf(file, "    return 0;\n}");

	fclose(file);
    
	return 0;
}
int write_cmake_lists(FILE* file, const char* project_name)
{
	if (fopen_s(&file, "CMakeLists.txt", "w") != 0) {
        perror("Failed to create CMakeLists.txt.");
        return 1;
    }
	
    fprintf(file, "cmake_minimum_required(VERSION 3.16)\n");
    fprintf(file, "project(%s)", project_name);

	fclose(file);
	
	return 0;
}
int write_cmb_config(FILE* file)
{
	if (fopen_s(&file, "cmb_config.json", "w") != 0) {
		perror("Failed to create cmb_config.json.");
		return 1;
	}

	fprintf(file, "[\n");
	fprintf(file, "    {}\n");
	fprintf(file, "]");

	fclose(file);
	
	return 0;
}
