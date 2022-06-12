BINARY = hall_of_fame
CC = gcc
CFLAGS = -g -no-pie -Wl,-z,relro

all:
	$(CC) $(CFLAGS) hall_of_fame.c -o $(BINARY)

clean:
	rm $(BINARY)
