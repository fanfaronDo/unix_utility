#ifndef COMMON_H
#define COMMON_H
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 4096
#define OPTION_SHORT '-'
#define OPTION_LONG "--"
#define FILES_SIZE 100
#define FILE_NAME_SIZE 50
#define ASCHII_SIZE 256

typedef enum errors {
  FILE_ERROR = -1,
  ALLOCATE_ERROR = -2,
  OPTION_ERROR = -3,
  NO_PATTERN = -4,
  COMPIL_REGEX_ERROR = -5,
  HELPER_USE = -6,
  IS_EMPTY = -7,

} errors;

void error_handler(int id_error);
FILE* open_file(char* file_name, bool is_message);
bool is_long_option(char* option);
bool is_short_option(char* option);
bool has_short_option(char* command, char* pattern);
void write_line(char* line, const char* table[ASCHII_SIZE]);
int get_length_file(FILE* file);
void set_table_aschi(const char* table[ASCHII_SIZE]);
void set_table_noneprinting_aschi(const char* table[ASCHII_SIZE]);

#endif
