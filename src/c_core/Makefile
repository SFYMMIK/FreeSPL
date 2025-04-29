CC = gcc
CFLAGS = -Wall -Wextra -O2
OBJ = main.o lexer.o parser.o executor.o error_handling.o token.o
TARGET = freespl

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)
