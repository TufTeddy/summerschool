#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int shm_fd;
    char* message_1 = "World";
    void* ptr;
    char* tmp;
    shm_fd = shm_open("SHM", O_RDWR, 0666);

    ptr = mmap(0, 1024, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("%s\n", (char*)ptr);
    tmp = (char*) ptr;
    ptr = mmap (0, 1024, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    ptr += strlen(tmp);
    sprintf(ptr, "%s", message_1);

    shm_unlink("SHM");
    return 0;
}
