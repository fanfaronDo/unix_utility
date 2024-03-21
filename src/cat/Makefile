CC=gcc
FLAGS=-Wall -Werror -Wextra
DIR_COMMON=../common
CAT_FILES=$(DIR_COMMON)/common.c *.c  
S21_CAT=s21_cat

all: $(S21_CAT)

$(S21_CAT):
	$(CC) $(FLAGS) $(CAT_FILES) -o $(S21_CAT)

clean:
	rm $(S21_CAT)

rebuild:
	$(MAKE) clean
	$(MAKE) all

