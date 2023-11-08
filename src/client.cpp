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

using namespace std;
using namespace cv;


void cut_image(Mat img, int x, int y, int width, int height, char * buf) {
	

	uint8_t* pixelPtr = (uint8_t*)img.data;
	int cn = img.channels();
	Scalar_<uint8_t> bgrPixel;
	
	//img.rows, img.cols
	int cnt = 0;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{	
			if (i >= img.rows || j >= img.cols) {
				bgrPixel.val[0] = 0;
				bgrPixel.val[1] = 0;
				bgrPixel.val[2] = 0;

			} else {

				bgrPixel.val[0] = pixelPtr[i*img.cols*cn + j*cn + 0]; // B
				bgrPixel.val[1] = pixelPtr[i*img.cols*cn + j*cn + 1]; // G
				bgrPixel.val[2] = pixelPtr[i*img.cols*cn + j*cn + 2]; // R
			}
		    cout << int(bgrPixel.val[0]) << " " << int(bgrPixel.val[0]) << " " << int(bgrPixel.val[0]) << "\n";
			buf[cnt++] = (char)bgrPixel.val[0]; 
		}
	}
}

void send_image(string filename ) {
	string image_path = "../data/panda_300_300.png";
    Mat img = imread(image_path, IMREAD_COLOR);

}

void wrap() {

}

void send(char* values, int values_len) {
	int sockfd; 
    char buffer[MAXLINE]; 

    struct sockaddr_in servaddr; 
   
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    memset(&servaddr, 0, sizeof(servaddr)); 
       
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY; 
       
    int n;
    socklen_t len; 
       
    sendto(sockfd, (const char *)values, values_len, 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
   
    close(sockfd); 
}



int main() {
	cout << "hello\n";
	
	char data[SQUARE_SIZE * SQUARE_SIZE];
	
	/*cut_image(0, 0, SQUARE_SIZE, SQUARE_SIZE, data);
	for (int i = 0; i < SQUARE_SIZE*SQUARE_SIZE; i++) {
		cout << ((int)data[i]) << " ";	
	}
	
	send(data, SQUARE_SIZE*SQUARE_SIZE);*/

	/*for(int i = 0; i < 10; i++) {
		send(data, 3);
	}*/

	return 0;
}
