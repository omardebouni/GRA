CC=gcc
CFLAGS=-O3 -Wall -Wextra -std=c17 -Wpedantic

.PHONY: clean	arsinh
all:	arsinh 
arsinh:	main.c	custom_math.c utility.c inverse_sinh.c alternatives.c
			$(CC)	$(CFLAGS)	-o	$@	$^	-lm	-g
clean:
			rm	-f	./arsinh 
