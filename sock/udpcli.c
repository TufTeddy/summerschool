#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(){
	struct sockaddr_in serv_addr;
	int sockfd, i, slen = sizeof(serv_addr), portnum = 8888;
	char buf[256];
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	printf ("Inpute msg: ");
	memset(buf, '0', sizeof(buf));
	fgets(buf, sizeof(buf), stdin);
	
	sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, slen);
	
	recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &slen);
	printf("Got back: %s", buf);

	
	return 0;
}