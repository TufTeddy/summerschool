#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

int main(){
	int sockfd, portnum = 8888, clilen, i=0;
	int sfd[5], tmp[5];
	char buf[256];
	pthread_t tid[5];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnum);
	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	if (sfd[i%5] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)){
		tmp[i%5] = sfd[i%5];
		pthread_create(&tid[i%5], NULL, rettime, &tmo[i%5]);
		i++;
	}
	bzero(buf, sizeof(buf));
	recv(sockfd1, buf, sizeof(buf)-1, 0);
	printf("got msg: %s", buf);
	for (n = 0; n < 10; n+=2){
		buf[n] = 'x';
		buf[n+1] = 'D';
	}
	send(sockfd1, buf, sizeof(buf)-1, 0);
}