#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>

#include "../common/common.h"
#include "../common/my_list.h"
#define BUFF_SIZE 4096

typedef struct {
  int pattern;
  int ignore_case;
  bool invert_match;
  bool count;
  bool files_with_matches;
  bool line_number;
  bool no_filename;
  bool no_messages;
  bool file_p;
  bool only_matching;

} options;

void s21_grep(int count_separate_command, char** command);
int find_patterns_and_files(options opt, node* list_patterns_head,
                            char** start_ptr, char** end_ptr);
char** find_default_pattern(char** pattern, char** end);
int write_patterns_from_file(node* list_patterns_head, int* count,
                             char* file_name);
int parse_command(options* opt, int count_separate_command, char** command,
                  node* list_patterns_head, int* count_patterns);
int default_pattern(options* opt, char** command, int size_command,
                    node* list_patterns_head, int* count_patterns);
bool is_file_availability(int size_command);
bool match_patterns(options opt, const char* string, node* list_patterns_head,
                    char* matches[BUFF_SIZE], int* error);
void read_from_stdin(options opt, node* list_patterns_head);
void read_from_files(options opt, node* list_patterns_head, int size_command,
                     char** command);
int read_file(FILE* file, char* file_name, int count_file,
              node* list_patterns_head, options opt);
void render_line(char* line, options opt, char* matches[BUFF_SIZE],
                 char* file_name, int count_file, int count_line);
void free_matches(char* matches[BUFF_SIZE]);
void render_no_line(options opt, int count_file, char* file_name,
                    int count_matched_line);
#endif