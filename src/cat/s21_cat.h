#ifndef S21_CAT_H
#define S21_CAT_H
#include "../common/common.h"

typedef struct options {
  bool number_nonblank;
  bool show_ends;
  bool number;
  bool squeeze_blank;
  bool show_tabs;
  bool show_noneprinting;

} options;

void s21_cat(int count_separate_command, char** command);
void s21_cat_read_from_stdin(const char* table[ASCHII_SIZE], options opt,
                             int* last_line_blank, int* line_number);
void s21_cat_read_from_files(const char* table[ASCHII_SIZE],
                             int count_separate_command, char** command,
                             options opt, int* last_line_blank,
                             int* line_number);
void read_from_file(const char* table[ASCHII_SIZE], FILE* fp, options opt,
                    int* last_line_blank, int* line_number, int* squeeze);
void formating_table_according_options(const char* table[ASCHII_SIZE],
                                       options opt);
void number_noneblank(int len, int* line_number);
void number(int* line_number);
void show_ends(const char* table[ASCHII_SIZE]);
void show_tabs(const char* table[ASCHII_SIZE]);
void show_noneprinting(const char* table[ASCHII_SIZE]);
bool squeeze_blank(int len, int* last_line_blank);

int parse_command(int count_separate_command, char** command, options* opt);
int set_long_option(char* command, options* opt);
int set_short_option(char* command, options* opt, int len_comand);
void set_show_tabs_optoin(options* opt);
void set_number_nonblank_option(options* opt);
void set_show_ends_option(options* opt);
void set_number_option(options* opt);

#endif