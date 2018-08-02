#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
 

int main(){
	mqd_t ds;
	char text[] = "Hello 2nd term";
	char new_text[128];
	int prio;
	struct mq_attr queue_attr;
	queue_attr.mq_maxmsg = 32; 
	queue_attr.mq_msgsize = 128; 
	ds = mq_open("/queue", O_CREAT | 0666, &queue_attr);
	mq_send(ds, text, strlen(text), 1);
	//sleep(10);
	//mq_receive(ds, new_text, sizeof(new_text), &prio);
	//printf("Msg: %s, pr = %d", new_text, prio);
	mq_close(ds);
	return 0;
}