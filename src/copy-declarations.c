#include "copy-declarations.h"

#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <sds/sds.h>
#include <stdio.h>
#include <unistd.h>

#define PUBLIC

sds get_contents(FILE* file) {
    fseek(file, 0, SEEK_SET);
    sds contents = sdsempty();
    for (int character; (character = getc(file)) != EOF; ) {
        assert(character < CHAR_MAX); // we don't support unicode yet
        contents = sdscatlen(contents, &character, 1);
    }
    fseek(file, 0, SEEK_SET);
    return contents;
}

bool starts_with(char* string, char* pattern) {
    for (int i = 0; pattern[i] != '\0'; i++) {
        if (string[i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

int line_length(char* string) {
    int length = 0;
    for (; string[length] != '\0'; length++) {
        if (string[length] == '\n') {
            return length + 1;
        }
    }
    return length;
}

PUBLIC int copy_declarations(char* source_filepath, char* header_filepath, bool dry_run) {
    FILE* source_file = fopen(source_filepath, "r");
    FILE* header_file = fopen(header_filepath, "r");

    if (source_file == NULL) {
        fprintf(stderr, "Error opening source file %s\n", source_filepath);
    }
    if (header_file == NULL) {
        fprintf(stderr, "Error opening header file %s\n", header_filepath);
    }
    if (source_file == NULL || header_file == NULL) {
        return 1;
    }

    int num_of_declarations = 0;
    sds declarations[MAX_PUBLIC_FUNCTIONS];

    sds source_contents = get_contents(source_file);
    for (char* line_start = source_contents; *line_start != '\0';) {
        size_t length = line_length(line_start);
        char* pattern = "PUBLIC";
        if (starts_with(line_start, pattern)) {
            sds line = sdsnewlen(line_start + strlen(pattern), length - strlen(pattern));
            sdstrim(line, "\n {");
            line = sdscat(line, ";\n");
            declarations[num_of_declarations] = line;
            num_of_declarations++;
        }
        line_start += length;
    }

    sds header_contents = get_contents(header_file);
    sds section_start_line = sdscatprintf(sdsempty(), "%s %s", HEADER_SECTION_START_PATTERN, source_filepath);
    sds section_end_line = sdscatprintf(sdsempty(), "%s %s", HEADER_SECTION_END_PATTERN, source_filepath);
    char* section_start = NULL;
    char* section_end = NULL;
    for (char* line_start = header_contents; *line_start != '\0';) {
        int length = line_length(line_start);
        if (section_start == NULL) {
            if (starts_with(line_start, section_start_line)) {
                section_start = line_start + length;
            }
        } else {
            if (starts_with(line_start, section_end_line)) {
                section_end = line_start;
            }
        }
        line_start += length;
    }

    if (section_start == NULL || section_end == NULL) {
        printf("Could not locate the section in the file, please add the following lines to the %s file\n", header_filepath);
        printf("%s \n", section_start_line);
        printf("%s \n", section_end_line);
        return 1;
    }

    sds result = sdsnewlen(header_contents, section_start - header_contents);
    for (int i = 0; i < num_of_declarations; i++) {
        result = sdscat(result, declarations[i]);
    }
    result = sdscat(result, section_end);

    if (dry_run) {
        printf("%s", result);
    } else {
        FILE *file = fopen(header_filepath, "w");
        if (!file) {
            fprintf(stderr, "Error opening file %s in write mode\n", header_filepath);
        }
        fputs(result, file);
        fclose(file);
    }

    return 0;
}