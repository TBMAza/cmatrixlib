CC := gcc

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS := -fsanitize=address,undefined
LDFLAGS := -lpthread

CMOBJDIR := matrixlibc/obj

tests: tests.o matrixlibcbuild
	@$(CC) -o tests tests.o $(CMOBJDIR)/matrixlibc.o $(CMOBJDIR)/matrixlibcutils.o $(LDFLAGS) $(SANFLAGS)

tests.o: tests.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o tests.o -c tests.c

matrixlibcbuild:
	@make -C matrixlibc/

run:
	@./tests

clean:
	@rm tests tests.o
	@make -C matrixlibc/ clean
