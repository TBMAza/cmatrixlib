CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS := -fsanitize=address,undefined
LDFLAGS := -lpthread

main: main.o cmatrixlib.o utils.o
	@$(CC) -o main main.o cmatrixlib.o utils.o $(LDFLAGS) $(SANFLAGS)

main.o: main.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o main.o -c main.c

cmatrixlib.o: cmatrixlib/cmatrixlib.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o cmatrixlib.o -c cmatrixlib/cmatrixlib.c

utils.o: utils/utils.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o utils.o -c utils/utils.c

cleano:
	@rm *.o

cleane:
	@rm main

run:
	@./main
