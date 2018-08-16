#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct tosend{
	short srcport;
	short dstport;
	short length;
	short csum;
	char data[56];
};

int main(){
	struct sockaddr_in serv_addr, sender;
	struct tosend tsend;

	int sockfd, slen = sizeof(sender);
	short portnum = 8888, srcnum = 6666;
	char data[56], getbuf[84];

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	memset(data, '0', sizeof(data));
	memset(getbuf, '0', sizeof(getbuf));

	
	tsend.srcport = htons(srcnum);
	tsend.dstport = htons(portnum);
	tsend.csum = 0;
	tsend.length = htons(64);
	strncpy(tsend.data, "Oh, hello there prick\0", sizeof(tsend.data));

	
		
		
	while (1){
		if (sendto(sockfd, &tsend, sizeof(tsend), 0, (struct sockaddr *)&serv_addr, slen) < 0){
		perror("Sendto");
		exit(1);
		}
	
		recvfrom(sockfd, getbuf, sizeof(getbuf), 0, (struct sockaddr *)&sender, &slen);
		struct udphdr *udph = (struct udphdr* )(getbuf+20);
		char *data = getbuf+28;		
		if (udph->dest == htons(srcnum)){
			printf("msg from: %s\t", inet_ntoa(sender.sin_addr));
			printf("msg: %s\n", data);
		}
		sleep(2);
	}
		
	return 0;
}