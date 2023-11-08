#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PORT    8080
#define MAXLINE 1024*1024
#define SQUARE_SIZE 30

#define IMAGE_WIDTH 300
#define IMAGE_HEIGHT 300
#define SQUARE_SIZE 30

#define IMG_ID 1
#define SEQ_ID 2



using namespace std;
using namespace cv;



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

    Mat img = Mat::zeros(Size(IMAGE_WIDTH, IMAGE_HEIGHT),CV_8UC1);
    uint8_t *pixelPtr = (uint8_t *) img.data;
    int cn = img.channels();
    Scalar_<uint8_t> bgrPixel;
    //print("img.channels: %d\n")

    for(int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            pixelPtr[y * img.cols * cn + x * cn + 0] = 255;
            pixelPtr[y * img.cols * cn + x * cn + 1] = 255;
            pixelPtr[y * img.cols * cn + x * cn + 2] = 255;

        }
    }
    imshow("Image", img);
    imwrite("../out_data/some.png", img);
    while (true) {

        n = recvfrom(sockfd, (char *) buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *) &cliaddr,
                     &len);
        //buffer[n] = '\0';
        int seq = buffer[1]*256 + buffer[2];

        int x = (seq * SQUARE_SIZE) % IMAGE_WIDTH;
        int y = ((seq * SQUARE_SIZE) / IMAGE_WIDTH) * SQUARE_SIZE;

        for (int i = IMG_ID+SEQ_ID; i < n; i++) {
            int real_i = (i - IMG_ID - SEQ_ID);
            int real_x = (real_i % SQUARE_SIZE) + x;
            int real_y = (real_i / SQUARE_SIZE) + y;

            pixelPtr[real_y * img.cols * cn + real_x * cn + 0] = buffer[i];
            pixelPtr[real_y * img.cols * cn + real_x * cn + 1] = buffer[i];
            pixelPtr[real_y * img.cols * cn + real_x * cn + 2] = buffer[i];


        }

        printf("size: %d (%d) %d %d (x=%d y=%d)\n", n, cnt++, buffer[0], seq, x, y);

        imshow("Image", img);
        imwrite("../out_data/some.png", img);

    }
    return 0;
}