CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS := -fsanitize=address,undefined
LDFLAGS := -lpthread

main: main.o cmatrixlib.o
	@$(CC) -o main main.o cmatrixlib.o $(LDFLAGS) $(SANFLAGS)
main.o: main.c
	@$(CC) $(CCFLAGS) $(SANFLAGS) -o main.o -c main.c
cmatrixlib.o: cmatrixlib.c
	@$(CC) $(CCFLAGS) $(SANFLAGS) -o cmatrixlib.o -c cmatrixlib.c
cleano:
	@rm *.o
cleane:
	@rm main
run:
	@./main
