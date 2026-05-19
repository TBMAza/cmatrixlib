CC := gcc

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS := -fsanitize=address,undefined

SRCDIR := src
OBJDIR := obj

build:
	@mkdir $(OBJDIR)
	@make matrixlibc.o
	@make matrixlibcutils.o

matrixlibc.o: $(SRCDIR)/matrixlibc.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o $(OBJDIR)/matrixlibc.o -c $(SRCDIR)/matrixlibc.c

matrixlibcutils.o: $(SRCDIR)/matrixlibcutils.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o $(OBJDIR)/matrixlibcutils.o -c $(SRCDIR)/matrixlibcutils.c

clean:
	@rm -rf $(OBJDIR)
