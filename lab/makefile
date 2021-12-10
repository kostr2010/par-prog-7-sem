CC=gcc
MPICC=mpicc
CFLAGS=-std=c99 -lm -Wall -Wextra -Werror

all: main 1g

main: main.c main-parallel.c
	$(CC) $(CFLAGS) main.c -o main.out
	$(MPICC) $(CFLAGS) main-parallel.c -o main-parallel.out

1g: 1g.c 1g-parallel.c
	$(CC) $(CFLAGS) 1g.c -o 1g.out
	$(MPICC) $(CFLAGS) 1g-parallel.c -o 1g-parallel.out

clean: clean_bin clean_exec

clean_bin:
	rm -f *.o

clean_exec:
	rm -f *.out