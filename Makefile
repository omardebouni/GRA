CC=gcc
CFLAGS=-O3 -Wall -Wextra -std=c17 -Wpedantic

.PHONY: clean	main
all:	main 
main:	main.c	custom_math.c
			$(CC)	$(CFLAGS)	-o	$@	$^	-lm	-g
clean:
			rm	-f	./main 
