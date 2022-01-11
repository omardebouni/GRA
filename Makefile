CC=gcc
CFLAGS=-O3 -Wall -Wextra -std=c17 -Wpedantic

.PHONY: clean	main
all:	main	correctness 
main:	main.c	custom_math.c
			$(CC)	$(CFLAGS)	-o	$@	$^	-lm	-g
			
correctness: correctness_check.c main.c
			$(CC)	$(CFLAGS)	-o	$@	$^	-lm	-g
clean:
			rm	-f	./main 
