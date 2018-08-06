#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(){
	int newsock = 0, sockfd1 = 0, sockfd2 = 0, portnum = 8888, addrlen = 0, exitflag = 0, epollfd, epollsize, i;
	struct sockaddr_in addr;
	
	struct epoll_event ev1, ev2;
	struct epoll_event events[2];
	epollfd = epoll_create1(0);
	char buf[1025];
	char msg[27] = "Oh hello there, u connected";
	sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd1 <= 0 || sockfd2 <= 0){
		perror ("Socket");
		exit(1);
	}
	
	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(portnum);
	if (bind(sockfd1, (struct sockaddr* )&addr, sizeof(addr)) < 0){
		perror ("bind");
		exit(1);
	}
	if (bind(sockfd2, (struct sockaddr* )&addr, sizeof(addr)) < 0){
		perror ("bind");
		exit(1);
	}
	if(listen(sockfd1, 5) < 0){
		perror("listen");
		exit(1);
	}
	
	ev1.data.fd = sockfd1;
	ev2.data.fd = sockfd2;
	ev1.events = EPOLLIN | EPOLLET;
	ev2.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd1, &ev1);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd2, &ev2);
	while (1){
		epollsize = epoll_wait(epollfd, events, 2, 0);
		for (i = 0; i < epollsize; ++i){
			if (events[i].data.fd == sockfd1){
				newsock = accept(sockfd1, (struct sockaddr*)&addr, &addrlen);
				send(newsock, msg, sizeof(msg), 0);
				recv(newsock, buf, sizeof(buf)-1, 0);
				printf("%s\n", buf);
				if (shutdown(sockfd1, SHUT_RDWR)){
					puts("tcp sock closed");
				}
				sockfd1 = 0;
			}
			if (events[i].data.fd == sockfd2){
				if (recvfrom(sockfd2, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&addr, &addrlen) > 0){
					printf ("%s\n", buf);
				}
				sendto(sockfd2, msg, sizeof(msg), 0, (struct sockaddr *)&addr, addrlen);
				sleep(2);
				sockfd2 = 0;
			}
		}
		if (!sockfd1 && !sockfd2){
			return 0;
		}
	}
}
