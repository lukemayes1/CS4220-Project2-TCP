#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> // bzero()
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()

#define SIZE 1024
#define PORT 8080

// Function to write file
void write_file(int sockfd) {
	int n;
	FILE *fp;
	char *filename = "recv.txt";
	char buffer[SIZE];

	fp = fopen(filename, "w");
	while(1) {
		n = recv(sockfd, buffer, SIZE, 0);
		if(n <= 0) {
			break;
			return;
		}

		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
	}

	return;
}

// Main function
int main() {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// Create socket and verify
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("[-]Socket creation failed.\n");
		exit(0);
	} else {
		printf("[+]Socket successfilly created.\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	// Assign IP and PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	// Bind newly created socket to given IP and verify
	if((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("[-]Socket bind failed.\n");
		exit(0);
	} else {
		printf("[+]Socket successfully binded.\n");
	}

	// Listen and verify
	if((listen(sockfd, 5)) != 0) {
		printf("[-]Listen failed.\n");
		exit(0);
	} else {
		printf("[+]Server listening.......\n");
	}
	len = sizeof(cli);

	// Accept data and verify
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
	if(connfd < 0) {
		printf("[-]Server accept failed.\n");
		exit(0);
	} else {
		printf("[+]Server successfully accpets the client.\n");
	}	

	// Call function to chat to client
	write_file(connfd);
	printf("[+]Data written to file successfully.\n");

	// Close the socket
	printf("[+]Closing connection.\n");
	close(sockfd);
}
