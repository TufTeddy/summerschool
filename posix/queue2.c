#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>

int main(){
	mqd_t ds;
	char text[] = "hello 1st";
	char new_text[256];
	struct mq_attr attr, old_attr;
	int prio;
	ds = mq_open("/queue", 0666, NULL);
	mq_receive(ds, new_text, sizeof(new_text), &prio);
	printf("Message: %s, prio = %d\n", new_text, prio);
	mq_send(ds, text, sizeof(text), 2);
	mq_close(ds);
	mq_unlink("/queue");
	return 0;
}