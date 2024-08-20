# Makefile - Build system for FreeSPL

CC = gcc
CFLAGS = -Wall -O2
TARGET = freespl

all: libcore.a $(TARGET)

libcore.a: lexer.o parser.o memory.o math.o
	ar rcs libcore.a lexer.o parser.o memory.o math.o

lexer.o: ../src/c_core/lexer.c
	$(CC) $(CFLAGS) -c ../src/c_core/lexer.c

parser.o: ../src/c_core/parser.c
	$(CC) $(CFLAGS) -c ../src/c_core/parser.c

memory.o: ../src/c_core/memory.c
	$(CC) $(CFLAGS) -c ../src/c_core/memory.c

math.o: ../src/c_core/math.c
	$(CC) $(CFLAGS) -c ../src/c_core/math.c

clean:
	rm -f *.o *.a $(TARGET)
