CC=gcc
FLAGS=-Wall -Werror -Wextra
DIR_COMMON=../common
GREP_FILES=$(DIR_COMMON)/common.c $(DIR_COMMON)/my_list.c *.c
S21_GREP=s21_grep

all:$(S21_GREP)

$(S21_GREP):
	$(CC) $(FLAGS) $(GREP_FILES) -o $(S21_GREP)

clean:
	rm $(S21_GREP)

rebuild:
	$(MAKE) clean
	$(MAKE) all

