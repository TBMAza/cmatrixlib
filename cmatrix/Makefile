CC := gcc

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS := -fsanitize=address,undefined

SRCDIR := src
OBJDIR := obj

build:
	@mkdir $(OBJDIR)
	@make cmatrixlib.o
	@make cmatrixutils.o

cmatrixlib.o: $(SRCDIR)/cmatrixlib.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o $(OBJDIR)/cmatrixlib.o -c $(SRCDIR)/cmatrixlib.c

cmatrixutils.o: $(SRCDIR)/cmatrixutils.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o $(OBJDIR)/cmatrixutils.o -c $(SRCDIR)/cmatrixutils.c

clean:
	@rm -rf $(OBJDIR)
