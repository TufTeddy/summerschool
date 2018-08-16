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
	unsigned char iphlen:4;
	unsigned char ipvers:4;
	unsigned char iptos;
	unsigned short iplength;
	unsigned short ipid;
	unsigned short ipflfr;
	unsigned char ipttl;
	unsigned char iptypeproto;
	unsigned short ipheadercsum;
	unsigned int ipsrc;
	unsigned int ipdst;
	unsigned short srcport;
	unsigned short dstport;
	unsigned short length;
	unsigned short csum;
	char data[56];
};


unsigned short get_chksum(unsigned short *ptr, int nbytes);

int main(){
	struct sockaddr_in serv_addr, sender;
	struct tosend tsend;

	int sockfd, slen = sizeof(sender), val = 1;
	short portnum = 8888, srcnum = 6666;
	char data[56], getbuf[84];

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL,(const char *) &val, sizeof(val));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	memset(data, '0', sizeof(data));
	memset(getbuf, '0', sizeof(getbuf));

	tsend.ipvers = 4;
	tsend.iphlen = 5;
	tsend.iptos = 0;
	tsend.iplength = 84;
	tsend.ipid = 100;
	tsend.ipflfr = 0;
	tsend.ipttl = 64;
	tsend.iptypeproto = 17;
	//tsend.ipheadercsum = 0;
	tsend.ipsrc = inet_addr("127.0.0.1");
	tsend.ipdst = inet_addr("127.0.0.1");
	tsend.srcport = htons(srcnum);
	tsend.dstport = htons(portnum);
	tsend.length = htons(64);
	strncpy(tsend.data, "Oh, hello there prick\0", sizeof(tsend.data));

	tsend.ipheadercsum = get_chksum((unsigned short*) &tsend, sizeof(tsend));
		
	while (1){
		if (sendto(sockfd, &tsend, sizeof(tsend), 0, (struct sockaddr *)&serv_addr, slen) < 0){
			perror("Sendto");
			exit(1);
		}
	
		if (recvfrom(sockfd, getbuf, sizeof(getbuf), 0, (struct sockaddr *)&sender, &slen) < 0){
			perror("recvfrom");
			exit(1);
		}
		struct udphdr *udph = (struct udphdr* )(getbuf+20);
		char *data = getbuf+28;		
		if (udph->dest == htons(srcnum)){
			printf("msg from: %s\t", inet_ntoa(sender.sin_addr));
			printf("msg: %s\n", data);
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