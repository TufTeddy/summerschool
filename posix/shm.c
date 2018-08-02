#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    mqd_t mq;   
	
	
	struct mq_attr ma;
	ma.mq_flags = 0;                // blocking read/write
    ma.mq_maxmsg = 16;              // maximum number of messages allowed in queue
    ma.mq_msgsize = sizeof(int);    // messages are contents of an int
    ma.mq_curmsgs = 0;              // number of messages currently in queue    

        
    int status = 0;
	char *snd = "Hello There";
	char *get;
	get = malloc(11);
    mq = mq_open("/test_queue", 0666 | O_CREAT, 0700, &ma);
	

	status = mq_send(mq, snd, sizeof(snd), 1);

	status = mq_receive(mq, get, sizeof(get), NULL);
	printf("%s", get);

    mq_close(mq);
    

    mq_unlink("test_queue");


    return status;

} 