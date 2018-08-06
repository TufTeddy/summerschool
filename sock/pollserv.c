#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(){
	int newsock = 0, sockfd1 = 0, sockfd2 = 0, portnum = 8888, addrlen = 0, exitflag = 0;
	struct sockaddr_in addr;
	
	struct pollfd pollset[2];
	
	fd_set readfds;
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
	
	pollset[0].fd = sockfd1;
	pollset[1].fd = sockfd2;
	pollset[0].events = POLLIN;
	pollset[1].events = POLLIN;
	pollset[0].revents = 0;
	pollset[1].revents = 0;
	
	addrlen = sizeof(addr);
	while (!exitflag){
		poll(pollset, 2, 0);
		if (pollset[0].revents & POLLIN){
			newsock = accept(sockfd1, (struct sockaddr*)&addr, &addrlen);
			send(newsock, msg, sizeof(msg), 0);
			recv(newsock, buf, sizeof(buf)-1, 0);
			printf("%s\n", buf);
			if (shutdown(sockfd1, SHUT_RDWR)){
				puts("tcp sock closed");
			}
			sockfd1 = 0;
		}
		if (pollset[1].revents & POLLIN){
			if (recvfrom(sockfd2, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&addr, &addrlen) > 0){
				printf ("%s\n", buf);
			}
			sendto(sockfd2, msg, sizeof(msg), 0, (struct sockaddr *)&addr, addrlen);
			sockfd2 = 0;
		}
		if (!sockfd1 && !sockfd2){
			exitflag = 1;
		}
	}
	return 0;
}