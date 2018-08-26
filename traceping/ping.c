#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip_icmp.h>

#include <arpa/inet.h>


#define PING_PKT_S 64

struct ping_pkt {
    struct icmphdr hdr;
    char msg[PING_PKT_S-sizeof(struct icmphdr)];
};
 
unsigned short checksum(void *b, int len) {    
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

int main(){
    struct sockaddr_in send_addr, rcv_addr;
	int sockfd, addrlen = sizeof(send_addr), portnum = 0, msg_count=0,
		i, addr_len, flag=1, msg_received_count=0, pingloop = 0;;
    struct ping_pkt pckt;
	
	send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons (portnum);
    send_addr.sin_addr.s_addr  = inet_addr("195.93.187.17");
 
    printf("\nTrying to connect to 'ngs.ru' IP: '195.93.187.17'\n");
 
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sockfd<0){
        perror("\nSocket");
        return 0;
    }
	
	while(pingloop < 5){
        
        flag=1;
      
        bzero(&pckt, sizeof(pckt));
         
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
         
        for ( i = 0; i < sizeof(pckt.msg)-1; i++ )
            pckt.msg[i] = i+'0';
         
        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
 
        if (sendto(sockfd, &pckt, sizeof(pckt), 0, 
		(struct sockaddr*) &send_addr, addrlen) <= 0){
            perror("sendto");
            flag=0;
        }
		
        if (recvfrom(sockfd, &pckt, sizeof(pckt), 0, 
		(struct sockaddr*)&rcv_addr, &addrlen) <= 0) {
            perror("\nrecvfrom");
		} else {
            if(flag) {
				printf("%d bytes from %s msg_seq=%d.\n",
						PING_PKT_S,inet_ntoa(rcv_addr.sin_addr), 
						msg_count);

				msg_received_count++;
			}
		}
		pingloop++;
		usleep(1000000);
    }
                    
    printf("\n==='195.93.187.17' ping statistics===\n");
    printf("\n%d packets sent, %d packets received, %d percent packet loss.\n", 
			msg_count, msg_received_count, 
			((msg_count - msg_received_count)/msg_count)); 
     
    return 0;
}