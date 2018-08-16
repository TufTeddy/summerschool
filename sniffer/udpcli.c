#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/udp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

void* msgsnd(	){
	char buf[256];
	struct sockaddr_in serv_addr;
	int udpfd, portnum = 8888, slen = sizeof(serv_addr);
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	
	printf ("Input msg: ");
	memset(buf, '0', sizeof(buf));
	fgets(buf, sizeof(buf), stdin);
	while (1){
	sendto(udpfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, slen);
	recvfrom(udpfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &slen);
	printf("%s", buf);
	sleep(5);
	}
}

int main(){
	struct sockaddr_in sender;
	pthread_t tid;
	int sockfd, slen = sizeof(sender);
	char buf[256];
	void *status;
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	
	
	memset(buf, '0', sizeof(buf));
	pthread_create(&tid, 0, &msgsnd, 0);
	
	while(1){
	
		if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&sender, &slen) > 0){
			printf("msg from: %s\n", inet_ntoa(sender.sin_addr));
		}
	}
	pthread_join(tid, &status);
	return 0;
}