#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf {
	long type;
	char str[60];
};

int main(){
	struct msgbuf msg;
	struct msgbuf msg1;
	int id, key;
	key = ftok("../arrays/array1.c", 'A');
	id = msgget(key, IPC_CREAT|0666);
	while (1){
		msgrcv(id, &msg1, sizeof(msg1), 10L, 0);
		msg.type = 1L;
		strcpy(msg.str, msg1.str);
		msgsnd(id, &msg, sizeof(msg), 0);
	}
	msgctl(id, IPC_RMID, 0);
	return 0;
}