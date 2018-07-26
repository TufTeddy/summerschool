#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(){

	int fd;
	char *fifo="./fifo.txt";
	mkfifo(fifo, 0666);
	char string[100];
	scanf("%s", string);
	fd = open(fifo, O_WRONLY);
	write(fd, string, sizeof(string)+1);
	close(fd);
	unlink(fifo);
	return 0;
}
