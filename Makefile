FILES = ./main.c ./src/jcstrc.c ./src/jcargc.c
OUT = main
DEBUG = -ggdb
COMPILER_FLAGS = -Wall -Wextra -Wpedantic -Wdouble-promotion -Wextra -Wswitch-default -Wswitch-enum -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn
COMPILER_IGNORE = -Wno-unused-function


all:
	gcc $(FILES) -o $(OUT) $(DEBUG) $(COMPILER_FLAGS) $(COMPILER_IGNORE)

ARGS = 
run:
	./main $(ARGS)

