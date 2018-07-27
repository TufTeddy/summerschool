#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf {
        long type;
        char str[20];
};

int main(){
        int id;
        int key;
        key = ftok("../arrays/array1.c", 'A');
        struct msgbuf msg;
        struct msgbuf msg1;
        id = msgget(key, 0);
	msgrcv(id, &msg1, sizeof(msg1), 10L, 0);
        printf("%s", msg1.str);

        msg.type = 1L;
        strcpy(msg.str, "Hi! i'm second");
        msgsnd(id, &msg, sizeof(msg), 0);
        return 0;
}
