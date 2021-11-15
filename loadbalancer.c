
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_NAME_LEN_MAX 255

#define SERVERPORT 6000
#define S1 6001
#define S2 6002
#define S3 6003

int clientNumber = 0;

typedef struct pthread_arg_t {
	int new_socket_fd;
	struct sockaddr_in client_address;
	
} pthread_arg_t;

void *pthread_initialize(void *arg);


int main(int argc, char *argv[]) {
	int port = SERVERPORT, socket_fd, new_socket_fd;
	struct sockaddr_in address;
	pthread_attr_t pthread_attr;
	pthread_arg_t *pthread_arg;
	pthread_t pthread;
	socklen_t client_address_len;

	memset(&address, 0, sizeof address);
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof address) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(socket_fd, 10) == -1) {
		perror("listen");
		exit(1);
	}


	/* Initialize pthread attribute to create detached threads. */
	if (pthread_attr_init(&pthread_attr) != 0) {
		perror("pthread_attr_init error");
		exit(1);
	}
	if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
		perror("pthread_attr_setdetachstate error");
		exit(1);
	}

	while (1) {
	
		pthread_arg = (pthread_arg_t *)malloc(sizeof *pthread_arg);
		if (!pthread_arg) {
			perror("malloc");
			continue;
		}

		/* Accept connection from client. */
		client_address_len = sizeof pthread_arg->client_address;
		new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
		if (new_socket_fd == -1) {
			perror("accept");
			free(pthread_arg);
			continue;
		}

		/* Initialize pthread arguments. */
		pthread_arg->new_socket_fd = new_socket_fd;
	
		if (pthread_create(&pthread, &pthread_attr, pthread_initialize, (void *)pthread_arg) != 0) {
			perror("pthread_create");
			free(pthread_arg);
			continue;
		}
	}

	return 0;
}

/*function to return load of server# idx */
int getLoads(int idx){
	
	/*initialize connection*/
	char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1\0";
	int server_port = 0, socket_fd;
	if(idx == 1) server_port = S1;
	else if(idx == 2) server_port = S2;
	else server_port = S3;
	
	struct hostent *server_host;
	struct sockaddr_in server_address;
	server_host = gethostbyname("localhost");
	memset(&server_address, 0, sizeof server_address);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

	/*make connection*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address);

	/*request load*/
	write(socket_fd, "__clients?", 100);

	/*get reply*/
	char reply[100];
	read(socket_fd, reply, 100);

	/*close connection*/
	close(socket_fd);

	/*convert string to integer*/
	int load = 0;
	sscanf(reply, "%d", &load);

	/*return load*/
	return load;
}

void sendquery(int idx, char input[], char reply[]){
	
	/*initialize connection*/
	char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1\0";
	int server_port = 0, socket_fd;
	if(idx == 1) server_port = S1;
	else if(idx == 2) server_port = S2;
	else server_port = S3;
	
	struct hostent *server_host;
	struct sockaddr_in server_address;
	server_host = gethostbyname("localhost");
	memset(&server_address, 0, sizeof server_address);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

	/*make connection*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address);

	write(socket_fd, input, 100);
	read(socket_fd, reply, 100);
}	

void *pthread_initialize(void *arg) {
	clientNumber++;
	int clientIdx = clientNumber;
	pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
	int new_socket_fd = pthread_arg->new_socket_fd;
	struct sockaddr_in client_address = pthread_arg->client_address;
	free(arg);

	printf("Client Number %d Connected\n\n", clientIdx);

	/*get input from actual client*/
	char input[100];
	read(new_socket_fd, input, 100);

	int load1 = getLoads(1);
	int load2 = getLoads(2);
	int load3 = getLoads(3);


	/*send query to the server with less load*/
	char reply[100];
	printf("\tLoads\n");
	if(load1 <= load2 && load1 <=load3)
	{ 
		printf("\n Server 1 : %d\n Server 2 : %d\n Server 3 : %d\n", load1+1, load2, load3);
		sendquery(1, input, reply);
	}
	else if (load2<=load1 && load2 <=load3)
	{
		printf("\n Server 1 : %d\n Server 2 : %d\n Server 3 : %d\n", load1, load2+1, load3);
		sendquery(2, input, reply);
	}
	else
	{
		printf("\n Server 1 : %d\n Server 2 : %d\n Server 3 : %d\n", load1, load2, load3+1);
		sendquery(3, input, reply);
	}
	
	/*send output to the client*/
	
	for(int i=0;i<100000000;i++)
	 {
	 	int c=i;
	 }
	write(new_socket_fd, reply, 100);

	printf("Client Number %d Disconnected\n\n", clientIdx);

	close(new_socket_fd);

	return NULL;
}

