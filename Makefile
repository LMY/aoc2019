DIR_SRC  = .
DIR_BIN  = .
SRCS    := $(wildcard $(DIR_SRC)/*.c)
TXT     := $(wildcard $(DIR_SRC)/*.txt)
OBJ     := $(SRCS:%.c=%.o)
EXE     := $(SRCS:%.c=%)

CFLAGS = $(shell pkg-config --cflags glib-2.0) -Wall -Wextra -Wpedantic -Werror
LDLIBS = $(shell pkg-config --libs glib-2.0) -lm

# main rule: create all executables
all: $(EXE)

# each exe depends upon its .o
$(EXE): $(DIR_BIN)/% : $(DIR_BIN)/%.o
	gcc -o $@ $^ $(LDLIBS)

# each .o depends upon its .c
$(OBJ): $(DIR_BIN)/%.o : $(DIR_SRC)/%.c
	gcc -c -o $@ $< $(CFLAGS)

run:
	@$(foreach exe, $(sort $(EXE)), echo "$(exe)"; $(exe); echo "";)

check-leaks:
	rm -f memleak.log
	@$(foreach exe, $(sort $(EXE)), valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(exe) >> memleak.log 2>&1;)

backup:
	tar -cf ../aoc-2019.tar Makefile $(SRCS) $(TXT) *.h

.PHONY: all clean run check-leaks backup

clean:
	rm -f $(OBJ) $(EXE) memleak.log
