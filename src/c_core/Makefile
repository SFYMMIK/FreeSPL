CC = gcc
CFLAGS = -Wall -Wextra -O2
OBJS = main.o lexer.o parser.o executor.o token.o error_handling.o debugger.o

freespl: $(OBJS)
	$(CC) $(CFLAGS) -o freespl $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) freespl
