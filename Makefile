FILES = .\src\main.c .\src\jcstring.c
OUT = main
DEBUG = -g
COMPILER_FLAGS = -Wall -Werror -Wpedantic -Wdouble-promotion -Wextra -Wswitch-default -Wswitch-enum -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn

all:
	gcc $(FILES) -o $(OUT) $(DEBUG) $(COMPILER_FLAGS)