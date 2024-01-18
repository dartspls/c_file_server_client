CC=gcc
CFLAGS=-l pthread
DEPS=common.h

all: server client

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

server: file_server.o
	$(CC) -o $@ $^ $(CFLAGS)

client: file_client.o
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o