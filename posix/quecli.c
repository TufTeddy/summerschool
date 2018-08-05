#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	mqd_t mq;
	int prio = 10;
	char buffer[1024];
	mq = mq_open("/queue", O_RDWR);

        snprintf(buffer, sizeof(buffer), "Msg from cli");
        mq_send(mq, buffer, sizeof(buffer), 0);
	mq_close(mq);

	return 0;
}
