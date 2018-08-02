#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <pthread.h>

struct msgbuf {
	long type;
	char str[60];
};

void* snd(void*);
void* gt(void*);

int main(){
	char *name;
	char tmp[10];
	printf("Input your nickname: ");
	scanf("%s", tmp);
	name = tmp;
	pthread_t snder, gtter;
	void *status;
	pthread_create (&snder, NULL, snd, name);
	pthread_create (&gtter, NULL, gt, NULL);
	pthread_join(snder, &status);
	pthread_join(gtter, &status);
	return 0;
}

void* snd(void *ptr){
	char in[50], out[60];
	char *vis = ": ";
	char *name = ptr;
	int id, key;
	struct msgbuf msg;
	key = ftok("../arrays/array1.c", 'A');
	id = msgget(key, 0);
	
	

	while(1){
		strcat(out, name);
		strcat(out, vis);
		fflush(stdin);
		//scanf("%s", in);
		fgets(in, sizeof(in), stdin);
		if (in == "exit"){
			exit(1);
		}
		strcat(out, in);
		memset(in, 0, sizeof(in));
		msg.type = 10L;
		strcpy(msg.str, out);
		msgsnd(id, &msg, sizeof(msg), 0);
		
		memset(out, 0, sizeof(out));
	}
}

void* gt(void *ptr){
	int id, key;
	struct msgbuf msg1;
	key = ftok("../arrays/array1.c", 'A');
	id = msgget(key, 0);
	while(1){
		msgrcv(id, &msg1, sizeof(msg1), 1L, 0);
		printf("%s\n", msg1.str);

	}
}