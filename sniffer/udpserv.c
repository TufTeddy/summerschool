#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
	struct sockaddr_in serv_addr, cli_addr;
	int sockfd, i, slen = sizeof(cli_addr), recv_len, portnum = 8888;
	char buf[56];
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	fflush(stdout);
	while(1){
		if( recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr, &slen)>0){
			printf("Got msg: %s\n", buf);
			fflush(stdout);
			sync();
			for (i = 0; i < 10; i += 2){
				buf[i] = 'x';
				buf[i+1] = 'D';
			}
			sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr, slen);
		}
	
	}
	return 0;
}