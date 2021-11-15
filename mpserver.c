
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOGS 10

int clientNumber = 0;
int numberOfConnections = 0;

typedef struct pthread_arg_t {
	int new_socket_fd;
	struct sockaddr_in client_address;

} pthread_arg_t;

/* Thread routine to serve connection to client. */
void *pthread_initialize(void *arg);


int main(int argc, char *argv[]) {
	int port, socket_fd, new_socket_fd;
	struct sockaddr_in address;
	pthread_attr_t pthread_attr;
	pthread_arg_t *pthread_arg;
	pthread_t pthread;
	socklen_t client_address_len;
	
		printf("Enter Port: ");
		scanf("%d", &port);

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

	if (listen(socket_fd, 10)== -1) {
		perror("listen");
		exit(1);
	}

	/* Initialize pthread attribute. */
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

		/* Accept connection to client. */
		client_address_len = sizeof pthread_arg->client_address;
		new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
		if (new_socket_fd == -1) {
			perror("accept");
			free(pthread_arg);
			continue;
		}

		/* Initialize pthread argument. */
		pthread_arg->new_socket_fd = new_socket_fd;
	
		if (pthread_create(&pthread, &pthread_attr, pthread_initialize, (void *)pthread_arg) != 0) {
			perror("pthread_create");
			free(pthread_arg);
			continue;
		}
	}

	return 0;
}

/*function to convert string to upper case*/
void upper_string(char s[]) {
	int c = 0;
	while (s[c] != '\0') {
		if (s[c] >= 'a' && s[c] <= 'z')
		s[c] = s[c] - 32;
	c++;
	}
	return;
}

void *pthread_initialize(void *arg) {
	/*initialize*/
	clientNumber++;
	int clientIdx = clientNumber;
	numberOfConnections++;
	pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
	int new_socket_fd = pthread_arg->new_socket_fd;
	struct sockaddr_in client_address = pthread_arg->client_address;


	free(arg);

	char string[100];
	read(new_socket_fd, string, 100);

	/*perform task based on the input*/
	if(strcmp("__clients?",string)==0){

		char result[100];
		sprintf(result,"%d",numberOfConnections-1);
		write(new_socket_fd, result, 100);
		clientNumber--;
	} else {
		printf("Client Number %d Connected\n", clientIdx);
		upper_string(string);
		/*close connection after some delay to get some time to connect other client*/
		for(int i=0; i<100000000;i++)
	  		for(int j=0;j<100;j++)
	  		{
	       			int c = j;
	  		} 
		write(new_socket_fd, string, 100); 
		
		   
		printf("Client Number %d Disconnected\n", clientIdx); 
	}

	
	close(new_socket_fd);
	numberOfConnections--;

	return NULL;
}

