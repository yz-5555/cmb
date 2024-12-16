#include "cmb_utils.h"
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

bool cd_here()
{
    char current_dir[MAX_PATH];

    if (CURRENT_DIR(current_dir) == 0) {
        fprintf(stderr, "cmb: Failed to get current directory.\n");
        return false;
    }
    if (CHDIR(current_dir) == 1) {
        fprintf(stderr, "cmb: Failed to cd to current directory.\n");
        return false;
    }
    return true;
}
inline bool is_flag(const char* arg)
{
    return arg[0] == '-';
}
FileState read_file(const char* path, char** buf)
{
    FILE* file = NULL;
    int result = fopen_s(&file, path, "r");

    if (result == ENOENT) {
        fprintf(stderr, "cmb_utils: Failed to find the file.\n");
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

    *buf = (char*)malloc(size + 1);
    if (*buf == NULL) {
        perror("cmb_utils: Failed to allocate memory, ");
        return CFS_ERROR;
    }

    fread(*buf, sizeof(char), size, file);
    (*buf)[size] = '\0';

    fclose(file);

    return CFS_SUCCESS;
}
void get_json_string(char** str, const cJSON* json)
{
    if (!cJSON_IsString(json) || json->valuestring == NULL) {
        // printf("get_json_string abort: json is invalid.\n");
        return;
    }
    if (*str != NULL && strcmp(*str, json->valuestring) == 0) {
        // printf("get_json_string abort: str has the same value already.\n");
        return;
    }

    size_t size = strlen(json->valuestring) + 1;
    *str = (char*)malloc(size);
	if (*str == NULL) {
		// printf("get_json_string abort: Failed to dynamically allocate a string.\n");
		return;
	}
    strcpy_s(*str, size, json->valuestring);

    // printf("get_json_string: Successfully assigned '%s' to str.\n", *str);
}
inline bool is_target(const char* preset_name, const char* target_name)
{
    // printf("is_target: Checking... input: '%s' vs target: '%s'\n", preset_name, target_name);
    return (target_name == NULL) || (strcmp(preset_name, target_name) == 0);
}
int get_preset(const char* path, const char* preset_name, Preset* preset)
{
    // printf("get_preset: Reading file...\n");
    char* buf = NULL;
    FileState state = read_file(path, &buf);
    if (state != CFS_SUCCESS) {
        fprintf(stderr, "cmb_utils: Failed to read cmb_presets.json.\n");
        return 1;
    }
    // printf("get_preset: Reading file: Pass\n");

    // printf("get_preset: Pasing JSON...\n");
    cJSON* json = cJSON_Parse(buf);
    if (json == NULL) {
        const char* err = cJSON_GetErrorPtr();
        if (err != NULL) {
            fprintf(stderr, "cmb_utils: Failed to parse cmb_presets.json: %s\n", err);
            cJSON_Delete(json);
            return 1;
        }
    }
    free(buf);
    // printf("get_preset: Parsing JSON: Pass\n");

    // printf("get_preset: Validating array...\n");
    if (!cJSON_IsArray(json)) {
        fprintf(stderr, "cmb_utils: Expected a JSON array in cmb_presets.json.\n");
        cJSON_Delete(json);
        return 1;
    }

    if (cJSON_GetArraySize(json) == 0) {
        fprintf(stderr, "cmb_utils: cmb_presets.json is empty.\n");
        cJSON_Delete(json);
        return 1;
    }
    // printf("get_preset: Validating array: Pass\n");

    // printf("get_preset: Reading values, looking for: '%s'\n", preset_name);
    cJSON* presetObj;
    cJSON_ArrayForEach(presetObj, json)
    {
        cJSON* name = cJSON_GetObjectItemCaseSensitive(presetObj, "name");
        get_json_string(&(preset->name), name);

        if (!is_target(preset->name, preset_name)) {
            // printf("get_preset: '%s' is not the object we looking for.\n", name->valuestring);
            continue;
        }
        // printf("get_preset: Yes! We found '%s' here!\n", name->valuestring);

        cJSON* compilers = cJSON_GetObjectItemCaseSensitive(presetObj, "compilers");
        if (compilers != NULL) {
            cJSON* c_compiler = cJSON_GetObjectItemCaseSensitive(compilers, "C");
            get_json_string(&(preset->c_compiler), c_compiler);

            cJSON* cpp_compiler = cJSON_GetObjectItemCaseSensitive(compilers, "CXX");
            get_json_string(&(preset->cpp_compiler), cpp_compiler);
        }
        cJSON* generator = cJSON_GetObjectItemCaseSensitive(presetObj, "generator");
        get_json_string(&(preset->generator), generator);

        cJSON* source_dir = cJSON_GetObjectItemCaseSensitive(presetObj, "source_dir");
        get_json_string(&(preset->source_dir), source_dir);

        cJSON* build_target = cJSON_GetObjectItemCaseSensitive(presetObj, "build_target");
        get_json_string(&(preset->build_target), build_target);

        cJSON* build_dir = cJSON_GetObjectItemCaseSensitive(presetObj, "build_dir");
        get_json_string(&(preset->build_dir), build_dir);

		cJSON* run = cJSON_GetObjectItemCaseSensitive(presetObj, "run");
		get_json_string(&(preset->run), run);

        // cJSON* generateCmd = cJSON_GetObjectItemCaseSensitive(presetObj, "generateCmd");
        // get array.
        // cJSON* buildCmd = cJSON_GetObjectItemCaseSensitive(presetObj, "buildCmd");
        // get array.
        //
        break;
    }
    // printf("get_preset: Reading values for '%s': Pass\n", preset->name);

    cJSON_Delete(json);
    return 0;
}
void free_preset(Preset* preset)
{
    if (preset == NULL)
        return;

    if (preset->name != NULL)
        free(preset->name);

    if (preset->generator != NULL)
        free(preset->generator);

    if (preset->c_compiler != NULL)
        free(preset->c_compiler);

    if (preset->cpp_compiler != NULL)
        free(preset->cpp_compiler);

    if (preset->source_dir != NULL && strcmp(preset->source_dir, ".") != 0)
        free(preset->source_dir);

    if (preset->build_target != NULL && strcmp(preset->build_target, "Debug") != 0)
        free(preset->build_target);

    if (preset->build_dir != NULL && strcmp(preset->build_dir, "target") != 0)
        free(preset->build_dir);

	if (preset->run != NULL && strcmp(preset->run, "run") != 0)
		free(preset->run);
}
int run_command(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	int size = vsnprintf(NULL, 0, format, args) + 1;

	va_end(args);

	if (size <= 0) return 1;

	char* command = (char*)malloc(size);
	if (command == NULL) return 1;

	va_start(args, format);
	vsnprintf(command, size, format, args);
	va_end(args);

	int result = system(command);
	free(command);

	return result;
}
