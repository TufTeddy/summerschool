#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(){
	struct sockaddr_in serv_addr;
	int sockfd, i, slen = sizeof(serv_addr), portnum = 8888;
	char buf[1024];
	char msg[16] = "It's UDP, HELLO";
	msg[15] = '\0';
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	

	
	sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&serv_addr, slen);
	
	recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &slen);
	printf("Got back: %s\n", buf);

	
	return 0;
}