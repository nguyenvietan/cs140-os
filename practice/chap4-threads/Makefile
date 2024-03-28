CC = gcc
CFLAGS = -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS
LDFLAGS = -lpthread

all: threadHello

threadHello: threadHello.o sthread.o
	$(CC) $(LDFLAGS) $^ -o $@

threadHello.o: threadHello.c
	$(CC) $(CFLAGS) -c $< -o $@

sthread.o: sthread.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f threadHello threadHello.o sthread.o
