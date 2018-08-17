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

int main(){
	struct sockaddr_in serv_addr, sender;
	struct udphdr *udph;
	int sockfd, slen = sizeof(sender);
	short portnum = 8888, srcnum = 6666;
	char buf[64], getbuf[84];
	char str[] = "Oh, hello there, prick";
	
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	memset(buf, '0', sizeof(buf));
	memset(getbuf, '0', sizeof(getbuf));
	
	strncpy(buf+sizeof(struct udphdr), str, sizeof(str));
	
	udph = (struct udphdr* )(buf);
	udph->source = htons(srcnum);
	udph->dest = htons(portnum);
	udph->len = htons(sizeof(struct udphdr)+sizeof(str));
	udph->check = 0;
	
		
	while (1){
		if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, slen) < 0){
		perror("Sendto");
		exit(1);
		}
	
		recvfrom(sockfd, getbuf, sizeof(getbuf), 0, (struct sockaddr *)&sender, &slen);
		udph = (struct udphdr* )(getbuf+20);
		char *data = getbuf+28;		
		if (udph->dest == htons(srcnum)){
			printf("msg from: %s\t", inet_ntoa(sender.sin_addr));
			printf("msg: %s\n", data);
		}
		sleep(2);
	}
		
	return 0;
}