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

unsigned short get_chksum(unsigned short *ptr, int nbytes);

int main(){
	struct sockaddr_in serv_addr, sender;
	struct udphdr *udph;
	struct iphdr *iph;

	int sockfd, slen = sizeof(sender);
	short portnum = 8888, srcnum = 5555;
	char buf[84], getbuf[100];
	char str[] = "Oh, hello there, maaan";

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	
	/*int val = 1;
	if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0){
		perror("setsockopt");
		exit(1);
	}*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	memset(buf, '0', sizeof(buf));
	memset(getbuf, '0', sizeof(getbuf));
	
	iph = (struct iphdr*)(buf);
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(str);
	iph->id = rand()%100;
	iph->frag_off = 0;
	iph->ttl = 64;
	iph->protocol = 17;
	iph->check = 0;
	iph->saddr = inet_addr("127.0.0.1");
	iph->daddr = inet_addr("127.0.0.1");

	udph = (struct udphdr* )(buf+sizeof(struct iphdr));
	udph->source = htons(srcnum);
	udph->dest = htons(portnum);
	udph->len = htons(sizeof(struct udphdr)+sizeof(str));
	udph->check = 0;

	strncpy(buf+sizeof(struct iphdr)+sizeof(struct udphdr), str, sizeof(str));
		
	while (1){
		if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, slen) < 0){
			perror("Sendto");
			exit(1);
		}
	
		recv(sockfd, getbuf, sizeof(getbuf), 0);
		printf("got it, got it, got it");
		udph = (struct udphdr* )(getbuf+sizeof(struct iphdr));
		char *data = getbuf+sizeof(struct iphdr)+sizeof(struct udphdr);		
		if (udph->dest == htons(srcnum)){
			printf("msg from: %s\t", inet_ntoa(sender.sin_addr));
			printf("msg: %s\n", data);
			fflush(stdout);
			sync();
		}
		//sleep(2);
	}
		
	return 0;
}

unsigned short get_chksum(unsigned short *ptr, int nbytes)
{
    register long sum;
    u_short oddbyte;
    register u_short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *) & oddbyte) = *(u_char *) ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return (answer);
}