#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
	struct sockaddr_in serv_addr, sender;
	struct udphdr *udph;
	struct iphdr *iph;
	
	int sockfd, slen = sizeof(sender);
	short portnum = 8888, srcnum = 6666;
	char buf[64], getbuf[84];
	char str[] = "Oh, hello there, prick";
	
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sockfd <= 0){
		perror("socket");
		exit(0);
	}
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
	
		
	if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, slen) < 0){
		perror("Sendto");
		exit(1);
	}
	while(1){
		recv(sockfd, getbuf, sizeof(getbuf), 0);
		iph = (struct iphdr* )(getbuf+0);
		udph = (struct udphdr* )(getbuf+20);
		char *data = getbuf+28;		
		if (htons(udph->source) == portnum){
			printf("msg from: %s:%d\t", inet_ntoa(*(struct in_addr* )&iph->saddr), htons(udph->source));
			printf("msg: %s\n", data);
			fflush(stdout);
			sync();
		}
	}
	return 0;
}