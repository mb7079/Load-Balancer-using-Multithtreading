
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_NAME_LEN_MAX 255

int main(int argc, char *argv[]) {

	char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1\0";
	int socket_fd;
	struct hostent *server_host;
	struct sockaddr_in server_address;

	server_host = gethostbyname(server_name);

	memset(&server_address, 0, sizeof server_address);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(6000);
	memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

	/* Create TCP socket. */
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* Connect to socket with server address. */
	if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address) == -1) {
		perror("connect");
		exit(1);
	}


	printf("Connection established to server\n");

	char string[100];
	printf("Message to server : ");
	scanf("%s",string);

	/*Write to server and get reply*/
	write(socket_fd, string, 100);
	read(socket_fd, string, 100);

	/*close connection*/
	close(socket_fd);

	printf("\nMessage from server : %s\n", string);
	return 0;
}
