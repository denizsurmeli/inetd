CC = gcc
CFLAGS = -w
all: server client inetd_client inetd service
mandatory: server client
bonus: inetd_client inetd service
server:
	$(CC) server.c -o server 
client:
	$(CC) client.c -o client
inetd_client: inetd
	$(CC) inetd_client.c -o inetd_client
inetd: service
	$(CC) -pthread inetd.c -o inetd
service:
	$(CC) service.c -o service
clean:
	rm -rf client server service inetd_client inetd
