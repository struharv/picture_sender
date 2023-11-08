#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

#define SQUARE_SIZE 30
#define IMG_ID 1
#define SEQ_ID 2

using namespace std;
using namespace cv;

char const * server_address = "127.0.0.1";
/**
 *
 * @param values
 * @param values_len
 */
void send_image_region(unsigned char *values, int values_len) {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(server_address);

    int n;
    socklen_t len;

    sendto(sockfd, (const char *) values, values_len,
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));

    close(sockfd);
}

void split_image(Mat img, int pos_x, int pos_y, int width, int height, unsigned char *buf, int buf_start) {
    auto *pixelPtr = (uint8_t *) img.data;
    int cn = img.channels();
    Scalar_<uint8_t> bgrPixel;

    int cnt = buf_start;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int real_x = x + pos_x*SQUARE_SIZE;
            int real_y = y + pos_y*SQUARE_SIZE;

            if (real_y >= img.rows || real_x >= img.cols) {
                // outside the region
                bgrPixel.val[0] = 0;
                bgrPixel.val[1] = 0;
                bgrPixel.val[2] = 0;
            } else {
                bgrPixel.val[0] = pixelPtr[real_y * img.cols * cn + real_x * cn + 0]; // B
                bgrPixel.val[1] = pixelPtr[real_y * img.cols * cn + real_x * cn + 1]; // G
                bgrPixel.val[2] = pixelPtr[real_y * img.cols * cn + real_x * cn + 2]; // R
            }

            buf[cnt++] = (char) ((bgrPixel.val[0]+bgrPixel.val[1]+bgrPixel.val[2])/3);
        }
    }
}

void send_image(string filename, unsigned char img_id) {
    Mat img = imread(filename, IMREAD_COLOR);

    int seq = 0;
    for (int y=0; y < img.rows / SQUARE_SIZE; y++) {
        for (int x=0; x < img.cols / SQUARE_SIZE; x++) {
            printf("sending %d %d\n", x, y);
            unsigned char data[SQUARE_SIZE * SQUARE_SIZE + IMG_ID + SEQ_ID];

            split_image(img, x, y, SQUARE_SIZE, SQUARE_SIZE, data, IMG_ID + SEQ_ID);
            data[1] = seq / 256;
            data[2] = seq % 256;

            send_image_region(data, SQUARE_SIZE * SQUARE_SIZE);
            seq++;
        }
    }
}

int main() {
    send_image("../data/panda_300_300.png", 1);

    return 0;
}
