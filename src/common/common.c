#include "common.h"

void error_handler(int id_error) {
  if (id_error == FILE_ERROR)
    fprintf(stderr, "No such file or directory \n");
  else if (id_error == ALLOCATE_ERROR) {
    fprintf(stderr, "Allocate memory error \n");
  } else if (id_error == OPTION_ERROR) {
    fprintf(stderr, "Invalid option\n");
  } else if (id_error == NO_PATTERN) {
    fprintf(stderr, "no pattern\n");
  } else if (id_error == COMPIL_REGEX_ERROR) {
    fprintf(stderr, "filed compil pattern\n");
  } else if (id_error == HELPER_USE) {
    fprintf(stderr, "use: s21_grep [options]... patterns [file]...\n");
  }
}

FILE *open_file(char *file_name, bool is_message) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL && is_message) {
    fprintf(stderr, "s21_grep: %s: ", file_name);
    error_handler(FILE_ERROR);
  }
  return file;
}

bool is_long_option(char *option) { return strspn(option, OPTION_LONG) == 2; }

bool is_short_option(char *option) { return option[0] == OPTION_SHORT; }

bool has_short_option(char *command, char *pattern) {
  int len_command = strlen(command);
  for (int i = 1; i < len_command; i++)
    if (!strchr(pattern, command[i])) return false;
  return true;
}

void write_line(char *line, const char *table[ASCHII_SIZE]) {
  int c;
  int len = strlen(line);
  for (int i = 0; i < len; i++) {
    c = line[i];
    printf("%s", table[c]);
  }
}

int get_length_file(FILE *file) {
  int size = 0;
  while (fgetc(file) != EOF) size++;
  return size;
}
void set_table_aschi(const char *table[ASCHII_SIZE]) {
  char *template_table[ASCHII_SIZE] = {
      "\00",  "\01",  "\02",  "\03",  "\04",  "\05",  "\06",  "\07",  "\b",
      "\t",   "\n",   "\v",   "\014", "\r",   "\016", "\017", "\020", "\021",
      "\022", "\023", "\024", "\025", "\026", "\027", "\030", "\031", "\032",
      "\033", "\034", "\035", "\036", "\037", " ",    "!",    "\"",   "#",
      "$",    "%",    "&",    "\'",   "(",    ")",    "*",    "+",    ",",
      "-",    ".",    "/",    "0",    "1",    "2",    "3",    "4",    "5",
      "6",    "7",    "8",    "9",    ":",    ";",    "<",    "=",    ">",
      "?",    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",
      "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",
      "Z",    "[",    "\\",   "]",    "^",    "_",    "`",    "a",    "b",
      "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",
      "l",    "m",    "n",    "o",    "p",    "q",    "r",    "s",    "t",
      "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",
      "~",    "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
      "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
      "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
      "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
      "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
      "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
      "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
      "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
      "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
      "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
      "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
      "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
      "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
      "\xfc", "\xfd", "\xfe", "\xff",
  };

  memcpy(table, template_table, sizeof template_table);
}

void set_table_noneprinting_aschi(const char *table[ASCHII_SIZE]) {
  const char *template_table_1[] = {"^@", "^A", "^B", "^C", "^D",
                                    "^E", "^F", "^G", "^H"};
  const char *template_table_2[] = {"^K", "^L", "^M", "^N",  "^O", "^P", "^Q",
                                    "^R", "^S", "^T", "^U",  "^V", "^W", "^X",
                                    "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  const char *template_table_3[] = {
      "^?",   "M-^@", "M-^A",  "M-^B", "M-^C", "M-^D", "M-^E", "M-^F", "M-^G",
      "M-^H", "M-^I", "M-^J",  "M-^K", "M-^L", "M-^M", "M-^N", "M-^O", "M-^P",
      "M-^Q", "M-^R", "M-^S",  "M-^T", "M-^U", "M-^V", "M-^W", "M-^X", "M-^Y",
      "M-^Z", "M-^[", "M-^\\", "M-^]", "M-^^", "M-^_", "M- ",  "M-!",  "M-\"",
      "M-#",  "M-$",  "M-%",   "M-&",  "M-'",  "M-(",  "M-)",  "M-*",  "M-+",
      "M-,",  "M--",  "M-.",   "M-/",  "M-0",  "M-1",  "M-2",  "M-3",  "M-4",
      "M-5",  "M-6",  "M-7",   "M-8",  "M-9",  "M-:",  "M-;",  "M-<",  "M-=",
      "M->",  "M-?",  "M-@",   "M-A",  "M-B",  "M-C",  "M-D",  "M-E",  "M-F",
      "M-G",  "M-H",  "M-I",   "M-J",  "M-K",  "M-L",  "M-M",  "M-N",  "M-O",
      "M-P",  "M-Q",  "M-R",   "M-S",  "M-T",  "M-U",  "M-V",  "M-W",  "M-X",
      "M-Y",  "M-Z",  "M-[",   "M-\\", "M-]",  "M-^",  "M-_",  "M-`",  "M-a",
      "M-b",  "M-c",  "M-d",   "M-e",  "M-f",  "M-g",  "M-h",  "M-i",  "M-j",
      "M-k",  "M-l",  "M-m",   "M-n",  "M-o",  "M-p",  "M-q",  "M-r",  "M-s",
      "M-t",  "M-u",  "M-v",   "M-w",  "M-x",  "M-y",  "M-z",  "M-{",  "M-|",
      "M-}",  "M-~",  "M-^?"};

  memcpy(table, template_table_1, sizeof template_table_1);
  memcpy(&table[9 + 2], template_table_2, sizeof template_table_2);
  memcpy(&table['~' + 1], template_table_3, sizeof template_table_3);
}

void free_array(int size, char **array) {
  for (int i = 0; i < size; i++) free(array[i]);
  free(array);
}
