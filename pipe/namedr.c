#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>

int main(){

        int fd;
        char *fifo="./fifo.txt";
        char output[100];
        fd = open(fifo, O_RDONLY);
        read(fd, output, sizeof(output)+1);
        close(fd);
	printf("Received string: %s\n", output);
	return 0;
}
