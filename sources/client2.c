#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#define MAX 200 
#define PORT 8080 
#define SA struct sockaddr 
  
int main() { 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 

    // Socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Socket ceration failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // Assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // Connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("Connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("Connected to the server..\n"); 
  
    // Function for chat 
    char buff[MAX]; 
    int n; 

    while (1) {
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) { 
            break; 
        }  
        printf("From Server : %s", buff); 

        bzero(buff, sizeof(buff)); 
        printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n'); 
        write(sockfd, buff, sizeof(buff));    
    } 
    
    // Close the socket 
    close(sockfd); 
} 

