DIR_CAT=./cat
DIR_GREP=./grep/
DIR_COMMON=./common/
S21_GREP=s21_grep
S21_CAT=s21_cat

all: $(S21_CAT) $(S21_GREP)

$(S21_CAT):
	cd $(DIR_CAT) && $(MAKE) $(S21_CAT) 

$(S21_GREP):
	cd $(DIR_GREP) && $(MAKE) $(S21_GREP)

clean:
	cd $(DIR_CAT) && $(MAKE) clean
	cd $(DIR_GREP) && $(MAKE) clean

rebuild:
	$(MAKE) clean
	$(MAKE) all


