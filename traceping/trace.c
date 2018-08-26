#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>

#include <arpa/inet.h>


unsigned short csum (void *b, int len) {
	unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;
 
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main () {
	struct sockaddr_in addr, addr2;
  	int hop = 0, val = 1, exitflag = 0, len = sizeof (addr);
	int sockfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
	char buf[4096], buff[4096];
	struct iphdr *iph;
	struct icmphdr *icmph, *icmph2;

	if (setsockopt (sockfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof (val)) < 0){
		perror("sockopt");
	}


	addr.sin_port = htons (0);
	addr.sin_family = AF_INET;
	inet_aton("195.93.187.17", &addr.sin_addr);

	while (!exitflag) {
		memset(buf, '0', sizeof(buf));
		memset(buff, '0', sizeof(buf));
		iph = (struct iphdr *) buf;
		iph->ihl = 5;
		iph->version = 4;
		iph->tos = 0;
		iph->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr);
		iph->id = rand()%100;
		iph->frag_off = 0;
		iph->ttl = hop;
		iph->protocol = IPPROTO_ICMP;
		iph->saddr = inet_addr("10.0.2.15");
		iph->daddr = inet_addr("195.93.187.17");
		iph->check = csum ((unsigned short *) buf, sizeof(iph));

		icmph = (struct icmphdr *) (buf + 20);
		icmph->type = ICMP_ECHO;
		icmph->code = 0;
		icmph->checksum = 0;
		icmph->un.echo.id = 0;
		icmph->un.echo.sequence = hop + 1;
		icmph->checksum = csum ((unsigned short *) (buf + 20), sizeof(icmph));
		
		sendto (sockfd, buf, sizeof(iph)+sizeof(icmph), 0, 
				(struct sockaddr*)&addr, len);


		
		recvfrom (sockfd, buff, sizeof(iph)+sizeof(icmph)+1, 0, 
					(struct sockaddr*) &addr2, &len);
		icmph2 = (struct icmphdr *) (buff + 20);
		
		if (icmph2->type != 0){
			printf ("hop num:%d Address:%s\n",hop,inet_ntoa(addr2.sin_addr));
		} else {
			printf ("Reached destination:%s with hop num:%d\n",
			inet_ntoa (addr2.sin_addr), hop);
			exitflag = 1;
		}
		hop++;
    }
	return 0;
}