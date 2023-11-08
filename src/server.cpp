#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#define PORT    8080
#define MAXLINE 1024*1024
#define SQUARE_SIZE 30

#define IMAGE_WIDTH 300
#define IMAGE_HEIGHT



using namespace std;

int main() {
    int sockfd;
    char buffer[MAXLINE];

    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address 
    if (bind(sockfd, (const struct sockaddr *) &servaddr,
             sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout << "Server is running \n";
    socklen_t len;
    int n;
    int cnt = 0;

    len = sizeof(cliaddr);
    while (true) {

        n = recvfrom(sockfd, (char *) buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *) &cliaddr,
                     &len);
        buffer[n] = '\0';
        int seq = buffer[1]*256 + buffer[2];
        printf("size: %d (%d) %d %d \n", n, cnt++, buffer[0], seq);


    }
    return 0;
}