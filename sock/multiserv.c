#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(){
	int newsock = 0, sockfd1 = 0, sockfd2 = 0, portnum = 8888, addrlen = 0, fdmax;
	struct sockaddr_in addr;
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
	
	addrlen = sizeof(addr);
	while (1){
		FD_ZERO(&readfds);
		FD_SET(sockfd1, &readfds);
		FD_SET(sockfd2, &readfds);
		if (sockfd1 > sockfd2){
			fdmax = sockfd1;
		} else {
			fdmax = sockfd2;
		}
		select(fdmax+1, &readfds, NULL, NULL, NULL);
		if (FD_ISSET(sockfd1, &readfds)){
			newsock = accept(sockfd1, (struct sockaddr*)&addr, &addrlen);
			send(newsock, msg, sizeof(msg), 0);
			recv(newsock, buf, sizeof(buf)-1, 0);
			printf("%s\n", buf);
			if (shutdown(sockfd1, SHUT_RDWR)){
				puts("tcp sock closed");
				sockfd1 = 0;
			}
		}
		if (FD_ISSET(sockfd2, &readfds)){
			if (recvfrom(sockfd2, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&addr, &addrlen) > 0){
				printf ("%s\n", buf);
			}
			sendto(sockfd2, msg, sizeof(msg), 0, (struct sockaddr *)&addr, addrlen);
			if (shutdown(sockfd2, SHUT_RDWR) == 0){
				puts("udp sock closed");
			}
			sockfd2 = 0;
		}
		if(!sockfd1 && !sockfd2){
			exit(0);
		}
	}
}