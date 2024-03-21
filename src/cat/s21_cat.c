#include "s21_cat.h"

int main(int argc, char** argv) {
  s21_cat(argc, argv);
  return 0;
}

void s21_cat(int count_separate_command, char** command) {
  options options = {false, false, false, false, false, false};
  int last_line_blank = '\n';
  int line_number = 1;
  const char* table[ASCHII_SIZE];
  set_table_aschi(table);
  int count_set_options =
      parse_command(count_separate_command, command, &options);
  formating_table_according_options(table, options);
  if (count_set_options < (count_separate_command - 1)) {
    s21_cat_read_from_files(table, count_separate_command, command, options,
                            &last_line_blank, &line_number);
  } else {
    s21_cat_read_from_stdin(table, options, &last_line_blank, &line_number);
  }
}

void s21_cat_read_from_stdin(const char* table[ASCHII_SIZE], options opt,
                             int* last_line_blank, int* line_number) {
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, sizeof(buffer), stdin)) {
    int len = strlen(buffer);
    if (squeeze_blank(len, last_line_blank) && opt.squeeze_blank) continue;
    if (opt.number_nonblank) number_noneblank(len, line_number);
    if (opt.number) number(line_number);

    write_line(buffer, table);
    buffer[0] = '\0';
  }
}

void s21_cat_read_from_files(const char* table[ASCHII_SIZE],
                             int count_separate_command, char** command,
                             options opt, int* last_line_blank,
                             int* line_number) {
  FILE* fp;
  int squeeze = 0;
  for (int i = 1; i < count_separate_command; i++) {
    int len_command = strlen(command[i]);
    int check_options = 1;
    if (check_options) {
      if (is_long_option(command[i])) {
        if (count_separate_command <= 2)
          s21_cat_read_from_stdin(table, opt, last_line_blank, line_number);
        else
          check_options = 0;
        continue;
      } else if (is_short_option(command[i])) {
        if (len_command == 1)
          s21_cat_read_from_stdin(table, opt, last_line_blank, line_number);
        continue;
      }
    }
    if ((fp = open_file(command[i], true)) == NULL) continue;

    read_from_file(table, fp, opt, last_line_blank, line_number, &squeeze);
    fclose(fp);
  }
}

int parse_command(int count_separate_command, char** command, options* opt) {
  int count_set_options = 0;
  int err_opt = 1;
  for (int i = 0; i < count_separate_command; i++) {
    int len_command = strlen(command[i]);
    if (is_long_option(command[i])) {
      if (len_command > 2) {
        err_opt *= set_long_option(command[i], opt);
        count_set_options++;
      }
    } else if (is_short_option(command[i])) {
      if (len_command > 1) {
        err_opt *= set_short_option(command[i], opt, len_command);
        count_set_options++;
      }
    }
    if (err_opt == 0) {
      error_handler(OPTION_ERROR);
      exit(1);
    }
  }
  return count_set_options;
}

int set_long_option(char* command, options* opt) {
  int err_opt = 1;
  if (strcmp(command, "--number") == 0)
    set_number_option(opt);
  else if (strcmp(command, "--number-nonblank") == 0)
    set_number_nonblank_option(opt);
  else if (strcmp(command, "--squeeze-blank") == 0)
    opt->squeeze_blank = true;
  else
    err_opt *= 0;

  return err_opt;
}

int set_short_option(char* command, options* opt, int len_command) {
  int err_opt = 1;
  for (int i = 1; i < len_command; ++i) {
    if (command[i] == 'b')
      set_number_nonblank_option(opt);
    else if (command[i] == 'E')
      opt->show_ends = true;
    else if (command[i] == 'e')
      set_show_ends_option(opt);
    else if (command[i] == 'n')
      set_number_option(opt);
    else if (command[i] == 's')
      opt->squeeze_blank = true;
    else if (command[i] == 'T')
      opt->show_tabs = true;
    else if (command[i] == 't')
      set_show_tabs_optoin(opt);
    else if (command[i] == 'v')
      opt->show_noneprinting = true;
    else
      err_opt *= 0;
  }

  return err_opt;
}

void set_show_tabs_optoin(options* opt) {
  opt->show_tabs = true;
  opt->show_noneprinting = true;
}

void set_number_nonblank_option(options* opt) {
  opt->number_nonblank = true;
  opt->number = false;
}

void set_show_ends_option(options* opt) {
  opt->show_ends = true;
  opt->show_noneprinting = true;
}

void set_number_option(options* opt) {
  if (!opt->number_nonblank) opt->number = true;
}

void read_from_file(const char* table[ASCHII_SIZE], FILE* fp, options opt,
                    int* last_line_blank, int* line_number, int* squeeze) {
  int c = 0;
  while (fread(&c, 1, 1, fp) > 0) {
    if (*last_line_blank == '\n') {
      if (opt.squeeze_blank && c == '\n') {
        if (*squeeze) continue;
        *squeeze = 1;
      } else
        *squeeze = 0;
      if (opt.number_nonblank)
        number_noneblank(c, line_number);
      else if (opt.number)
        number(line_number);
    }
    if (!*table[c])
      printf("%c", '\0');
    else
      printf("%s", table[c]);
    *last_line_blank = c;
  }
}

void formating_table_according_options(const char* table[ASCHII_SIZE],
                                       options opt) {
  if (opt.show_ends) show_ends(table);
  if (opt.show_tabs) show_tabs(table);
  if (opt.show_noneprinting) show_noneprinting(table);
}

void number_noneblank(int c, int* line_number) {
  if (c != '\n') number(line_number);
}

void number(int* line_number) {
  printf("%6i\t", *line_number);
  *line_number += 1;
}

void show_ends(const char* table[ASCHII_SIZE]) { table['\n'] = "$\n"; }

void show_tabs(const char* table[ASCHII_SIZE]) { table['\t'] = "^I"; }

void show_noneprinting(const char* table[ASCHII_SIZE]) {
  set_table_noneprinting_aschi(table);
}

bool squeeze_blank(int len, int* last_line_blank) {
  int current_line_blank = (len <= 1) ? 1 : 0;
  if (*last_line_blank && current_line_blank) return true;
  *last_line_blank = current_line_blank;
  return false;
}
