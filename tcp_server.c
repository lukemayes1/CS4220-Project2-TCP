#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> // bzero()
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()

#define MAX 80
#define PORT 8080

// Function designed for chat between client and server
void clientChat(int connfd) {
	char buff[MAX];
	int n;

	// Infinite loop for chat
	for(;;) {
		bzero(buff, MAX);

		// Read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));

		// Print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;

		// Copy server message in the buffer
		while((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		write(connfd, buff, sizeof(buff));
		
		/* Not sure if we need this */
		// If msg contains "exit" then server will exit and chat ended.
		if(strncmp("exit", buff, 4) == 0) {
			printf("Server exit...\n");
			break;
		}	
	}		 
}


// Main function
int main() {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// Create socket and verify
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	} else {
		printf("Socket successfilly created...\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	// Assign IP and PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	// Bind newly created socket to given IP and verify
	if((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Socket bind failed...\n");
		exit(0);
	} else {
		printf("Socket successfully binded...\n");
	}

	// Listen and verify
	if((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	} else {
		printf("Server listening..\n");
	}
	len = sizeof(cli);

	// Accept data and verify
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
	if(connfd < 0) {
		printf("Server accept failed...\n");
		exit(0);
	} else {
		printf("Server successfully accpets the client...\n");
	}	

	// Call function to chat to client
	clientChat(connfd);

	// Close the socket
	close(sockfd);
}
