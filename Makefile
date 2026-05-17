main: main.o cmatrixlib.o
	@gcc -o main main.o cmatrixlib.o -lpthread -fsanitize=address -g
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
