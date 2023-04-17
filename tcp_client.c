#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define SIZE 1024
#define PORT 8080

// Function to send file to TCP server
void sendFile(FILE *fp, int sockfd) {
    int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL) {
        if(send(sockfd, data, sizeof(data), 0) == -1) {
            printf("[-]Error in sending file.\n");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

// Main function
int main(int argc, char *argv[]) {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    FILE *fp;
 
    // Create socket and verify
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("[-]Socket creation failed.\n");
        exit(0);
    } else {
        printf("[+]Socket successfully created.\n");
    }
    bzero(&servaddr, sizeof(servaddr));
 
    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // Connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("[-]Connection with the server failed.\n");
        exit(0);
    } else {
        printf("[+]Connected to the server.\n");
    }

    // Open file
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("[-]Error in reading file.");
    }
 
    // Send file
    sendFile(fp, sockfd);
    printf("[+]File data sent successfully.\n");
 
    // Close the socket
    printf("[+]Closing connection.\n");
    close(sockfd);
}