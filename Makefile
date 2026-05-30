CFLAGS = -Wall -Wextra -std=c99
LDLIBS = -ledit -lm

clisp: clisp.c mpc.c
	$(CC) $(CFLAGS) clisp.c mpc.c $(LDLIBS) -o clisp