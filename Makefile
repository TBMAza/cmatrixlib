CC := gcc

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS := -fsanitize=address,undefined
LDFLAGS := -lpthread

CMSRCDIR := cmatrix/src
CMOBJDIR := cmatrix/obj

tests: tests.o cmatrixbuild
	@$(CC) -o tests tests.o $(CMOBJDIR)/cmatrixlib.o $(CMOBJDIR)/cmatrixutils.o $(LDFLAGS) $(SANFLAGS)

tests.o: tests.c
	@$(CC) $(CFLAGS) $(SANFLAGS) -o tests.o -c tests.c

cmatrixbuild:
	@make -C cmatrix/

run:
	@./tests

clean:
	@rm tests.o
	@make -C cmatrix/ clean
