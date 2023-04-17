CC=gcc

all:
	$(CC) tcp_client.c -o tcp_client
	$(CC) tcp_server.c -o tcp_server
clean:
	rm -f tcp_client tcp_server