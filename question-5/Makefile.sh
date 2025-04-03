CC = gcc
CFLAGS = -Wall -pthread

all: server client

server: chat_server.c common.h
	$(CC) $(CFLAGS) chat_server.c -o chat_server

client: chat_client.c common.h
	$(CC) $(CFLAGS) chat_client.c -o chat_client

clean:
	rm -f chat_server chat_client
