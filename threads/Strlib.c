#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

pthread_key_t key;
pthread_once_t once=PTHREAD_ONCE_INIT;
void dest(void* buf){
	free (buf);
}

void key_create(void){
	pthread_key_create(&key, dest);
}

char* Strerror (int errnum){
	char *buf;
	pthread_once(&once, key_create);
	buf = pthread_getspecific(key);
	if (!buf){
		buf = malloc(100);
		pthread_setspecific(key, buf);
	}
	strerror_r(errnum, buf, sizeof(buf));
	return buf;
}