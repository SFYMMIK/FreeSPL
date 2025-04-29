CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = main.c lexer.c parser.c error_handling.c executor.c token.c
OBJ = $(SRC:.c=.o)

all: freespl

freespl: $(OBJ)
	$(CC) $(CFLAGS) -o freespl $(OBJ)

clean:
	rm -f *.o freespl
