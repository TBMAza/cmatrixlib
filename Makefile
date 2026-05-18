main: main.o cmatrixlib.o
	@gcc -o main main.o cmatrixlib.o -lpthread -std=c99 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g
main.o: main.c
	@gcc -o main.o -c main.c
cmatrixlib.o: cmatrixlib.c
	@gcc -o cmatrixlib.o -c cmatrixlib.c
cleano:
	@rm *.o
cleane:
	@rm main
run:
	@./main
