#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using namespace std;


void process() {
    //printf("process");
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    char buffer[5000];
    struct sockaddr_in dst;
    int addr_len = sizeof(struct sockaddr_in);
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = INADDR_ANY;
    dst.sin_port = htons(12346);
    bind(fd, (struct sockaddr*)&dst, addr_len);
    listen(fd, 1024);
    cout<<"TCP Listen on: "<<12346<<endl;
    int readStatus = 0;
    int len = 0;
    while(1) {
        struct sockaddr_in cli_addr;
        char buff[20000];
        int len = sizeof(struct sockaddr);
        cout<<"wait for new connection.."<<endl;
	int ac_fd = accept(fd,(struct sockaddr*)&cli_addr,(socklen_t*)&len);
	cout<<"connection "<< inet_ntoa(cli_addr.sin_addr)<<":"<<htons(cli_addr.sin_port)<<" established!"<<endl;
	while(1) {
            readStatus = recv(ac_fd, buff, 20000, 0);
	    if (readStatus<=0) {
	    	if(readStatus==0) {
		    cout<<"connection closed!"<<endl;
		}
		break;
	    }
            buff[readStatus] = '\0';
	    cout<<"recv message["<<readStatus<<"]:"<<endl;
            cout<<buff<<" <<====== "<< inet_ntoa(cli_addr.sin_addr)<<":"<<htons(cli_addr.sin_port)<<endl;
	    cout<<"send message["<<readStatus<<"]:"<<endl;
	    cout<<buff<<" ======>> "<< inet_ntoa(cli_addr.sin_addr)<<":"<<htons(cli_addr.sin_port)<<endl;
	    send(ac_fd, buff, readStatus, 0);
	}
    }
}

int main() {
	std::thread recv_th = std::thread(process);
	recv_th.join();

	return 0;
}
