#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>

#define DEST_MAC0	0x08
#define DEST_MAC1	0x00
#define DEST_MAC2	0x27
#define DEST_MAC3	0x8c
#define DEST_MAC4	0xe4
#define DEST_MAC5	0x5f

unsigned short csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main(){
	struct sockaddr_ll serv_addr;
	struct sockaddr_in sender;
	struct udphdr *udph;
	struct iphdr *iph;
	struct ether_header *eth;
	
	
	int sockfd, slen = sizeof(serv_addr);
	short portnum = 8888, srcnum = 6666;
	char buf[128], getbuf[128];
	char str[] = "Oh, hello there, prick";
	char ethint[] = "enp0s3";
	
	struct ifreq if_idx;
	struct ifreq if_mac;
	memset(&if_idx, 0, sizeof(struct ifreq));
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ethint, sizeof(ethint));
	strncpy(if_mac.ifr_name, ethint, sizeof(ethint));
	
	
	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sockfd <= 0){
		perror("socket");
		exit(0);
	}
	/*
	int val = 1;
	if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0){
		perror("setsockopt");
		exit(1);
	}*/
		/* Get the INDEX address of the interface to send on */
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
		perror("SIOCGIFHWADDR");
	
	/* Index of the network device */
	serv_addr.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	serv_addr.sll_halen = 6;
	/* Destination MAC */
	serv_addr.sll_addr[0] = DEST_MAC0;
	serv_addr.sll_addr[1] = DEST_MAC1;
	serv_addr.sll_addr[2] = DEST_MAC2;
	serv_addr.sll_addr[3] = DEST_MAC3;
	serv_addr.sll_addr[4] = DEST_MAC4;
	serv_addr.sll_addr[5] = DEST_MAC5;
	
	memset(buf, '0', sizeof(buf));
	memset(getbuf, '0', sizeof(getbuf));
	
	strncpy(buf+sizeof(struct udphdr)+sizeof(struct iphdr)+sizeof(struct ether_header), str, sizeof(str));
	
	udph = (struct udphdr* )(buf+sizeof(struct iphdr)+sizeof(struct ether_header));
	udph->source = htons(srcnum);
	udph->dest = htons(portnum);
	udph->len = htons(sizeof(struct udphdr)+sizeof(str));
	udph->check = 0;
	
	iph = (struct iphdr*)(buf+sizeof(struct ether_header));
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(str);
	iph->id = rand()%100;
	iph->frag_off = 0;
	iph->ttl = 64;
	iph->protocol = 17;
	iph->check = 0;
	iph->saddr = inet_addr("10.8.240.2");
	iph->daddr = inet_addr("10.8.240.3");
	
	eth = (struct ether_header* )buf;
	eth->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eth->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eth->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eth->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eth->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eth->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eth->ether_dhost[0] = DEST_MAC0;
	eth->ether_dhost[1] = DEST_MAC1;
	eth->ether_dhost[2] = DEST_MAC2;
	eth->ether_dhost[3] = DEST_MAC3;
	eth->ether_dhost[4] = DEST_MAC4;
	eth->ether_dhost[5] = DEST_MAC5;
	eth->ether_type = htons(ETH_P_IP);
	
		
	/* Length of UDP payload and header */
	udph->len = htons(sizeof(struct udphdr)+sizeof(str));
	/* Length of IP payload and header */
	iph->tot_len = htons(sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(str));
	/* Calculate IP checksum on completed header */
	iph->check = csum((unsigned short *)(buf+sizeof(struct ether_header)), sizeof(struct iphdr)/2);
		
	if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, slen) < 0){
		perror("Sendto");
		exit(1);
	}
	while(1){
		recv(sockfd, getbuf, sizeof(getbuf), 0);
		iph = (struct iphdr* )(getbuf+sizeof(struct ether_header));
		udph = (struct udphdr* )(getbuf+sizeof(struct ether_header)+sizeof(struct iphdr));
		char *data = getbuf+sizeof(ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
		if (htons(udph->source) == portnum){
			printf("msg from: %s:%d\t", inet_ntoa(*(struct in_addr* )&iph->saddr), htons(udph->source));
			printf("msg: %s\n", data);
			fflush(stdout);
			sync();
		}
	}
	return 0;
}
