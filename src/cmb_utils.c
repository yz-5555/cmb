#include "cmb_utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// do-while is needed for semicolons.
#define OOPS(x)       \
    do {              \
        if (x) {      \
            return 1; \
        }             \
    } while (0)

#include "cJSON.h"

int find_end_of_options(int argc, char* argv[])
{
    int result = -1;
    if (argc > CMD_NAME_IDX + 1) {
        for (int i = 0; i < argc; i += 1) {
            if (!is_flag(argv[i]))
                continue;
            if (strcmp(argv[i], "--") != 0)
                continue;
            result = i;
        }
    }
    return result;
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
        return CMB_FS_NOENT;
    }

    if (result != 0 && result != ENOENT) {
        perror("cmb_utils: Failed to read the file, ");
        return CMB_FS_ERROR;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size == 0) {
        fprintf(stderr, "cmb_utils: The file is empty.\n");
        return CMB_FS_EMPTY;
    }

    *buf = (char*)malloc(size + 1);
    if (*buf == NULL) {
        perror("cmb_utils: Failed to allocate memory, ");
        return CMB_FS_ERROR;
    }

    fread(*buf, sizeof(char), size, file);
    (*buf)[size] = '\0';

    fclose(file);

    return CMB_FS_SUCCESS;
}
JsonState get_json_string(char** str, const cJSON* json)
{
    if (!cJSON_IsString(json) || json->valuestring == NULL) {
        // printf("get_json_string warning: json is invalid.\n");
        return CMB_JS_WARNING;
    }

    if (*str != NULL) {
        if (strcmp(*str, json->valuestring) == 0) {
            // printf("get_json_string warning: str has the same value already.\n");
            return CMB_JS_WARNING;
        }
    }

    size_t size = strlen(json->valuestring) + 1;
    *str = (char*)malloc(size);
    if (*str == NULL) {
        // printf("get_json_string error: Failed to dynamically allocate a string.\n");
        return CMB_JS_ERROR;
    }
    strcpy_s(*str, size, json->valuestring);

    // printf("get_json_string success: Successfully assigned '%s' to str.\n", *str);
    return CMB_JS_SUCCESS;
}
JsonState get_json_array(char*** arr, size_t* len, const cJSON* json)
{
    if (!cJSON_IsArray(json)) {
        // printf("get_json_array warning: json is invalid.\n");
        return CMB_JS_WARNING;
    }

    size_t size = cJSON_GetArraySize(json);
    *len = size;
    if (size <= 0) {
        // printf("get_json_array warning: json is invalid.\n");
        return CMB_JS_WARNING;
    }
    *arr = (char**)malloc(size * sizeof(char*));
    if (*arr == NULL) {
        // printf("get_json_array error: Failed to dynamically allocate an array.\n");
        return CMB_JS_ERROR;
    }

    for (int i = 0; i < size; i++) {
        (*arr)[i] = NULL;
    }

    for (int i = 0; i < size; i += 1) {
        cJSON* item = cJSON_GetArrayItem(json, i);
        JsonState state = get_json_string(&((*arr)[i]), item);

        if (state == CMB_JS_WARNING) {
            (*arr)[i] = (char*)malloc(1);

            if ((*arr)[i] == NULL) {
                // printf("get_json_array error: Failed to dynamically allocate a string.\n");
                return CMB_JS_ERROR;
            } else {
                (*arr)[i][0] = '\0';
            }
        } else if (state == CMB_JS_ERROR) {
            for (int j = 0; j < i; j += 1) {
                free((*arr)[j]);
            }
            free(*arr);
            *len = 0;

            // printf("get_json_array error: Got an error from get_json_string.\n");
            return CMB_JS_ERROR;
        }
    }
    // printf("get_json_array success: Successfully assigned values to arr.\n");
    return CMB_JS_SUCCESS;
}
inline bool is_target(const char* preset_name, const char* target_name)
{
    // printf("is_target: Checking... input: '%s' vs target: '%s'\n", preset_name, target_name);
    return (target_name == NULL) || (strcmp(preset_name, target_name) == 0);
}
int get_preset(const char* preset_name, Preset* preset)
{
    // printf("get_preset: Reading file...\n");
    char* buf = NULL;
    FileState state = read_file(CMB_PRESETS_PATH, &buf);
    if (state != CMB_FS_SUCCESS) {
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
        OOPS(get_json_string(&(preset->name), name) == CMB_JS_ERROR);

        if (!is_target(preset->name, preset_name)) {
            printf("get_preset: '%s' is not the object we looking for.\n", name->valuestring);
            continue;
        }
        // printf("get_preset: Yes! We found '%s' here!\n", name->valuestring);

        cJSON* compilers = cJSON_GetObjectItemCaseSensitive(presetObj, "compilers");
        if (compilers != NULL) {
            cJSON* c_compiler = cJSON_GetObjectItemCaseSensitive(compilers, "C");
            OOPS(get_json_string(&(preset->c_compiler), c_compiler) == CMB_JS_ERROR);

            cJSON* cpp_compiler = cJSON_GetObjectItemCaseSensitive(compilers, "CXX");
            OOPS(get_json_string(&(preset->cpp_compiler), cpp_compiler) == CMB_JS_ERROR);
        }
        cJSON* generator = cJSON_GetObjectItemCaseSensitive(presetObj, "generator");
        OOPS(get_json_string(&(preset->generator), generator) == CMB_JS_ERROR);

        cJSON* source_dir = cJSON_GetObjectItemCaseSensitive(presetObj, "source_dir");
        OOPS(get_json_string(&(preset->source_dir), source_dir) == CMB_JS_ERROR);

        cJSON* build_target = cJSON_GetObjectItemCaseSensitive(presetObj, "build_target");
        OOPS(get_json_string(&(preset->build_target), build_target) == CMB_JS_ERROR);

        cJSON* build_dir = cJSON_GetObjectItemCaseSensitive(presetObj, "build_dir");
        OOPS(get_json_string(&(preset->build_dir), build_dir) == CMB_JS_ERROR);

        cJSON* run = cJSON_GetObjectItemCaseSensitive(presetObj, "run");
        OOPS(get_json_string(&(preset->run), run) == CMB_JS_ERROR);

        cJSON* generate_cmd = cJSON_GetObjectItemCaseSensitive(presetObj, "generate_cmd");
        OOPS(get_json_array(&(preset->generate_cmd), &(preset->generate_cmd_size), generate_cmd) == CMB_JS_ERROR);

        cJSON* build_cmd = cJSON_GetObjectItemCaseSensitive(presetObj, "build_cmd");
        OOPS(get_json_array(&(preset->build_cmd), &(preset->build_cmd_size), build_cmd) == CMB_JS_ERROR);

        cJSON* run_cmd = cJSON_GetObjectItemCaseSensitive(presetObj, "run_cmd");
        OOPS(get_json_array(&(preset->run_cmd), &(preset->run_cmd_size), run_cmd) == CMB_JS_ERROR);

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

    if (preset->run != NULL)
        free(preset->run);

    if (preset->generate_cmd != NULL && preset->generate_cmd_size > 0) {
        for (int i = 0; i < preset->generate_cmd_size; i += 1) {
            free((preset->generate_cmd)[i]);
        }
        free(preset->generate_cmd);
    }
    if (preset->build_cmd != NULL && preset->build_cmd_size > 0) {
        for (int i = 0; i < preset->build_cmd_size; i += 1) {
            free((preset->build_cmd)[i]);
        }
        free(preset->build_cmd);
    }
    if (preset->run_cmd != NULL && preset->run_cmd_size > 0) {
        for (int i = 0; i < preset->run_cmd_size; i += 1) {
            free((preset->run_cmd)[i]);
        }
        free(preset->run_cmd);
    }

    if (preset->source_dir != NULL && strcmp(preset->source_dir, ".") != 0)
        free(preset->source_dir);

    if (preset->build_target != NULL && strcmp(preset->build_target, "Debug") != 0)
        free(preset->build_target);

    if (preset->build_dir != NULL && strcmp(preset->build_dir, "target") != 0)
        free(preset->build_dir);
}
int run_command(const char* format, int argc, char* argv[], size_t cmd_len, char* cmd[], ...)
{
    va_list args;
    va_start(args, cmd);

    int format_size = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    if (format_size <= 0) {
        fprintf(stderr, "cmb_utils: Failed to get the size of parameters.\n");
        return 1;
    }

    size_t params_size = 0;
    int end_of_options = -1;

    if (argc > CMD_NAME_IDX + 1) {
        end_of_options = find_end_of_options(argc, argv);
        if (end_of_options > 0) {
            for (size_t i = end_of_options + 1; i < argc; i += 1) {
                params_size += strlen(argv[i]) + 1;
            }
        }
    }
    if (cmd_len > 0) {
        for (size_t i = 0; i < cmd_len; i += 1) {
            params_size += strlen(cmd[i]) + 1;
        }
    }

    size_t total_size = format_size + params_size;
    char* command = (char*)malloc(total_size);
    if (command == NULL) {
        fprintf(stderr, "cmb_utils: Failed to allocate memory.\n");
        return 1;
    }

    va_start(args, cmd);
    vsnprintf(command, format_size, format, args);
    va_end(args);

    if (argc > CMD_NAME_IDX + 1) {
        if (end_of_options > 0) {
            for (size_t i = end_of_options + 1; i < argc; i += 1) {
                strcat_s(command, total_size, " ");
                strcat_s(command, total_size, argv[i]);
            }
        }
    }
    if (cmd_len > 0) {
        for (size_t i = 0; i < cmd_len; i += 1) {
            strcat_s(command, total_size, " ");
            strcat_s(command, total_size, cmd[i]);
        }
    }

    printf("> %s\n", command);

    int result = system(command);
    free(command);

    return result;
}
