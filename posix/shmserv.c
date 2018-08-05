#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h> 
#include <sys/mman.h>
#include <unistd.h>

int main(){
    	char* message_0 = "Hello";
	int shm_fd;
	void* ptr;

    	shm_fd = shm_open("SHM", O_CREAT | O_RDWR, 0666);
    	ftruncate(shm_fd, 1024);
    	ptr = mmap(0, 1024, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	sprintf(ptr, "%s", message_0);
	sleep(10);
	ptr = mmap(0, 1024, PROT_READ, MAP_SHARED, shm_fd, 0);
    	ptr += strlen(message_0);
	printf("%s\n", (char*)ptr);
    	return 0;
}
