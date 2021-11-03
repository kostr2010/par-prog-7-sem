CC=gcc
CFLAGS=-Wall -Werror -Wextra -fopenmp -g

SRC=bugged1 bugged2 bugged3 bugged4 bugged5 bugged6
all: $(SRC)

$(SRC): %: %.c
	$(CC) $(CFLAGS) -o $@.out $<