#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
	struct sockaddr_in serv_addr, cli_addr;
	int sockfd, i, slen = sizeof(cli_addr), recv_len, portnum = 8888;
	char buf[256];
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	fflush(stdout);
	while(1){
	recv_len = recvfrom(sockfd, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&cli_addr, &slen);
	printf("Got msg: %s", buf);
	for (i = 0; i < 10; i += 2){
		buf[i] = 'x';
		buf[i+1] = 'D';
	}
	sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr, slen);
	
	}
	return 0;
}