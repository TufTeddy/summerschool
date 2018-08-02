#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
 
#define MAXSIZE     27
 
void die(char *s)
{
    perror(s);
    exit(1);
}

void* getit(void* ptr);
void* sndit(void* ptr);
 
int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
	void *status;
	pthread_t snder, getter;
 
    key = ftok("./chat.c", 'T');
 
    if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");
 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
        die("shmat");
	
 

    s = shm;
 
    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
 
 	pthread_create(&getter, NULL, getit, shm);
	pthread_create(&snder, NULL, sndit, shm);
 
    pthread_join(getter, &status);
	pthread_join(snder, &status);
 

 
    exit(0);
}
void* getit(void* ptr){
	char* s;
	char* shm = ptr;
	while(1){
		for (s = shm; *s != '\0' || *s != '*'; s++)
			putchar(*s);
		putchar('\n');
		*shm = '*';
		while (*shm == '*')
			sleep(1);
	}
    
}

void* sndit(void* ptr){
	char* s;
	char* shm = ptr;
	int i;
	s = shm;
	char name[10], in[17], out[28];
	printf ("Input Your name: ");
	scanf("%s", name);
	strcat(out, name);
	out[27] = '\0';
	while (1){
		fgets(in, 17, stdin);
		strcat(out, in);
		//*shm = out[0];
		for (i = 0; i < sizeof(out); i++)
			*s++ = out[i];
		memset(out, 0, sizeof(out));
		memset(in, 0, sizeof(in));
	}
}