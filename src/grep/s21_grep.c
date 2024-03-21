#include "s21_grep.h"

int main(int argc, char** argv) {
  s21_grep(argc, argv);
  return 0;
}

void s21_grep(int size_command, char** command) {
  if (size_command <= 1) {
    error_handler(HELPER_USE);
    exit(1);
  }
  int error = 0;
  int count_patterns = 0;
  options opt = {0, 0, false, false, false, false, false, false, false, false};
  node* list_patterns_head = malloc(sizeof(node));
  if (list_patterns_head == NULL) error += 1;
  if (!error)
    error = parse_command(&opt, size_command, command, list_patterns_head,
                          &count_patterns);
  if (!error) {
    if (!(is_file_availability(size_command))) {
      read_from_stdin(opt, list_patterns_head);
    } else {
      read_from_files(opt, list_patterns_head, size_command, command);
    }
  }
  if (error == IS_EMPTY)
    free(list_patterns_head);
  else
    deep_free_list(list_patterns_head);

  if (error) error_handler(error);
}

int parse_command(options* opt, int size_command, char** command,
                  node* list_patterns_head, int* count_patterns) {
  int error = 0;
  const char* pattern_options = "e:ivclnhsof:?";
  int current_option =
      getopt_long(size_command, command, pattern_options, NULL, 0);
  for (; current_option != -1;
       current_option =
           getopt_long(size_command, command, pattern_options, NULL, 0)) {
    switch (current_option) {
      case 'e':
        opt->pattern = REG_EXTENDED;
        char* ptrn = strdup(optarg);
        add_elem(list_patterns_head, ptrn, *count_patterns);
        *count_patterns += 1;
        break;
      case 'i':
        opt->ignore_case = REG_ICASE;
        break;
      case 'v':
        opt->invert_match = true;
        break;
      case 'c':
        opt->count = true;
        break;
      case 'l':
        opt->files_with_matches = true;
        break;
      case 'n':
        opt->line_number = true;
        break;
      case 'h':
        opt->no_filename = true;
        break;
      case 's':
        opt->no_messages = true;
        break;
      case 'f':
        opt->file_p = true;
        error = write_patterns_from_file(list_patterns_head, count_patterns,
                                         optarg);
        break;
      case 'o':
        opt->only_matching = true;
        break;
      case '?':
        error = HELPER_USE;
        break;
      default:
        error = OPTION_ERROR;
    }
    if (error) break;
  }
  if (!error)
    error = default_pattern(opt, command, size_command, list_patterns_head,
                            count_patterns);
  if (!error)
    if (*count_patterns == 0) error = IS_EMPTY;

  return error;
}

int default_pattern(options* opt, char** command, int size_command,
                    node* list_patterns_head, int* count_patterns) {
  int error = 0;
  if (!(opt->pattern) && !(opt->file_p)) {
    char** pattern = find_default_pattern(&command[1], &command[size_command]);
    if (pattern == &command[size_command]) {
      error = IS_EMPTY;
    }
    if (!error) {
      char* ptrn = strdup(*pattern);
      add_elem(list_patterns_head, ptrn, *count_patterns);
      *count_patterns += 1;
      optind++;
    }
  }
  return error;
}

char** find_default_pattern(char** pattern, char** end) {
  for (; pattern != end && pattern[0][0] == '-'; pattern += 1)
    ;
  return pattern;
}

bool is_file_availability(int size_command) {
  int amount_files = size_command - optind;
  return amount_files > 0;
}

int write_patterns_from_file(node* list_patterns_head, int* count,
                             char* file_name) {
  int error = 0;
  FILE* f = NULL;
  if ((f = open_file(file_name, true)) == NULL) error = IS_EMPTY;
  if (!error) {
    char* line = 0;
    size_t len = 0;
    while ((getline(&line, &len, f)) != -1) {
      int length = strlen(line);
      if (length > 0) {
        char* pattern = strdup(line);
        if (pattern[length - 1] == '\n') pattern[length - 1] = '\0';

        add_elem(list_patterns_head, pattern, *count);
        *count += 1;
      } else
        continue;
    }
    free(line);
    fclose(f);
  }

  return error;
}

void read_from_files(options opt, node* list_patterns_head, int size_command,
                     char** command) {
  int error = 0;
  bool is_last_file = false;
  int count_file = size_command - optind;
  for (int i = optind; i < size_command; i++) {
    FILE* file_ptr;
    if (i == size_command - 1) is_last_file = true;
    char* file_name = command[i];
    file_ptr = open_file(file_name, !opt.no_messages);
    if (is_last_file && file_ptr == NULL) error = FILE_ERROR;
    if (file_ptr != NULL && !error)
      error =
          read_file(file_ptr, file_name, count_file, list_patterns_head, opt);
    else
      continue;
    fclose(file_ptr);
    if (error) break;
  }
}

int read_file(FILE* file, char* file_name, int count_file,
              node* list_patterns_head, options opt) {
  int error = 0;
  char* line = 0;
  size_t len = 0;
  int count_line = 0;
  int count_matched_line = 0;
  while (getline(&line, &len, file) != -1) {
    count_line++;
    char* matches[BUFF_SIZE] = {NULL};
    if (match_patterns(opt, line, list_patterns_head, matches, &error)) {
      count_matched_line++;
      if (!opt.count && !opt.files_with_matches)
        render_line(line, opt, matches, file_name, count_file, count_line);
    }
    free_matches(matches);
    if (error) break;
  }
  if (!error) render_no_line(opt, count_file, file_name, count_matched_line);
  free(line);

  return error;
}

bool match_patterns(options opt, const char* string, node* list_patterns_head,
                    char* matches[BUFF_SIZE], int* error) {
  int res = 0;
  int index_match = 0;
  node* current = list_patterns_head;
  while (current != NULL) {
    char* pattern = current->string;
    regex_t regex;
    int compil = opt.file_p ? REG_EXTENDED : opt.ignore_case | opt.pattern;
    if (regcomp(&regex, pattern, compil) != 0) {
      regfree(&regex);
      *error = COMPIL_REGEX_ERROR;
    }
    if (!(*error)) {
      regmatch_t match;
      size_t offset = 0;
      size_t str_len = strlen(string);

      for (int ret; (ret = regexec(&regex, string + offset, 1, &match, 0)) == 0;
           index_match++) {
        int len = match.rm_eo - match.rm_so;
        matches[index_match] = malloc(len + 1);
        if (matches[index_match] == NULL) {
          *error = ALLOCATE_ERROR;
          break;
        }
        memcpy(matches[index_match], string + match.rm_so + offset, len);
        matches[index_match][len] = '\0';
        offset += match.rm_eo;
        if (offset > str_len) break;
      }
      matches[index_match] = NULL;
      current = current->next;
      regfree(&regex);
    }
    if (*error) break;
  }
  if (!(*error)) res = opt.invert_match ? !(index_match) : index_match;
  return res;
}

void render_line(char* line, options opt, char* matches[BUFF_SIZE],
                 char* file_name, int count_file, int count_line) {
  int len = strlen(line);

  if (!(opt.files_with_matches))
    if (count_file > 1 && !(opt.count) && !(opt.no_filename) &&
        !(opt.only_matching))
      printf("%s:", file_name);

  if (opt.line_number && !(opt.only_matching)) printf("%d:", count_line);

  if (opt.only_matching && !(opt.invert_match)) {
    for (int i = 0; matches[i]; i++) {
      if (count_file > 1 && !(opt.no_filename)) printf("%s:", file_name);
      if (opt.line_number) printf("%d:", count_line);
      printf("%s\n", matches[i]);
    }
  }
  if (!(opt.only_matching && !(opt.invert_match)) &&
      !(opt.only_matching && opt.invert_match)) {
    if (line[len - 1] == '\n')
      printf("%s", line);
    else
      printf("%s\n", line);
  }
}

void render_no_line(options opt, int count_file, char* file_name,
                    int count_matched_line) {
  if (!opt.files_with_matches && opt.count && !(opt.no_filename) &&
      count_file > 1)
    printf("%s:", file_name);
  if (opt.count && !(opt.files_with_matches))
    printf("%d\n", opt.files_with_matches ? count_matched_line > 0
                                          : count_matched_line);
  if (opt.files_with_matches && count_matched_line) printf("%s\n", file_name);
}

void read_from_stdin(options opt, node* list_patterns_head) {
  int error = 0;
  char* line = 0;
  size_t len = 0;
  int count_line = 0;
  int count_matched_line = 0;

  while (getline(&line, &len, stdin) != -1) {
    count_line++;
    char* matches[BUFF_SIZE] = {NULL};
    if (match_patterns(opt, line, list_patterns_head, matches, &error)) {
      count_matched_line++;
      if (opt.line_number) printf("%d:", count_line);
      if (opt.only_matching && !(opt.invert_match)) {
        for (int i = 0; matches[i]; i++) printf("%s\n", matches[i]);
      } else {
        if (line[len - 1] == '\n')
          printf("%s", line);
        else
          printf("%s\n", line);
      }
    }
    free_matches(matches);
    if (error) break;
  }
  free(line);
}

void free_matches(char* matches[BUFF_SIZE]) {
  for (int i = 0; matches[i]; i++) free(matches[i]);
}