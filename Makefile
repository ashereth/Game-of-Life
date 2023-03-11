SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g

all: life

life: life.o 
	$(CC) -o life -lm life.o -lncurses

life.o: life.c  
	$(CC) $(CFLAGS) -c life.c

clean:
	rm -f *.o life

format:
	clang-format -i -style=file *.[ch]

