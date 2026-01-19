# RUN_PREF=valgrind
# AUX_FLAGS=-fsanitize=address

.PHONY: help
help:
	@echo 'make help'
	@echo 'make test_all'
	@echo 'make test_yes'
	@echo 'make test_no'
	@echo 'make clean'

CC := gcc
CFLAGS := -I../ -Wall -Wextra -Wfatal-errors -g $(AUX_FLAGS)

CMPL := $(CC)

COMMON_DEP := test.c test_*.ic
COMMON_DEP += ../ezjit.c ../ezjit.h ../impl/*.ic ../impl/arch/*.ic

.PHONY: test_all
test_all: test_no test_yes

# <no>
.PHONY: test_no
test_no: test_no_compile test_no_run

.PHONY: test_no_compile
test_no_compile: ./test_no.bin

./test_no.bin: $(COMMON_DEP)
	$(CMPL) ../ezjit.c test.c -o test_no.bin -DEZJIT_ARCH_NONE $(CFLAGS)

.PHONY: test_no_run
test_no_run:
	./test_no.bin
# </no>

# <yes>
.PHONY: test_yes
test_yes: test_yes_compile test_yes_run

.PHONY: test_yes_compile
test_yes_compile: ./test_yes.bin

./test_yes.bin: $(COMMON_DEP) test_yes.ic
	$(CMPL) ../ezjit.c test.c -o test_yes.bin $(CFLAGS)

.PHONY: test_yes_run
test_yes_run:
	./test_yes.bin
# </yes>

.PHONY: clean
clean:
	rm -f ./test*.bin
