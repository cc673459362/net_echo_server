#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using namespace std;


void process() {
    printf("process");
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    char buffer[5000];
    struct sockaddr_in dst;
    int addr_len = sizeof(struct sockaddr_in);
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = INADDR_ANY;
    dst.sin_port = htons(12345);
    bind(fd, (struct sockaddr*)&dst, addr_len);
    int readStatus = 0;
    int len = 0;
    while(1) {
        struct sockaddr_in cli_addr;
        char buff[20000];
        int len = sizeof(struct sockaddr);
	cout<<"wait for recv.."<<endl;
        readStatus = recvfrom(fd, buff, 20000, 0, (struct sockaddr*)&cli_addr, (socklen_t*)&len);
        //printf("readStatus:%d\n", readStatus);
        buff[readStatus] = '\0';
	cout<<"recv message["<<readStatus<<"]:"<<endl;
        cout<<buff<<" <<====== "<< inet_ntoa(cli_addr.sin_addr)<<":"<<htons(cli_addr.sin_port)<<endl;
	cout<<"send message["<<readStatus<<"]:"<<endl;
	cout<<buff<<" ======>> "<< inet_ntoa(cli_addr.sin_addr)<<":"<<htons(cli_addr.sin_port)<<endl;
	sendto(fd, buff, readStatus, 0, (struct sockaddr*)&cli_addr, len);
    }
}

int main() {
	std::thread recv_th = std::thread(process);
	recv_th.join();

	return 0;
}
