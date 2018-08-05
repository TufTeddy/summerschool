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
	struct mq_attr attr;
    	char buffer[1025];

	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 1024;
	attr.mq_curmsgs = 0;

	mq = mq_open("/queue", O_CREAT | O_RDWR, 0666, &attr);

	while(1) {
		memset(buffer, 0, sizeof(buffer));
        	if (mq_receive(mq, buffer, sizeof(buffer), 0) > 0){
			printf("CLIENT: %s\n", buffer);
			sleep(1);
			break;
		}
	}

	mq_close(mq);
	mq_unlink("/queue");

	return 0;
}
