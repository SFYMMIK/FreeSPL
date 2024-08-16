CC=gcc
CFLAGS=-Wall -Wextra -Iinclude
LDFLAGS=-lgtk-3 -lcurl

SRC=src/lexer.c src/parser.c src/codegen.c src/main.c src/utils.c src/extrafunc.c
OBJ=$(SRC:.c=.o)

all: my_language

my_language: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) my_language
