#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <string.h>


int main()
{
    int sockfd, portnum = 8888, n;

    struct sockaddr_in serv_addr;

    char buffer[1025];
	char msg[14] = "Got you, serv";
	msg[13] = '\0';
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(portnum);
    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	bzero(buffer,1025);
    recv(sockfd,buffer, sizeof(buffer), 0);
	printf ("%s\n", buffer);
    send(sockfd, msg, sizeof(msg), 0);
    return 0;
}