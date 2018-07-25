#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "Strlib.c"

void* func(){
	int n = rand()%101;
	char *buf;
	buf = malloc(100);
	buf = Strerror(n);
	printf("%s\n", buf);
}


int main(){
	srand(time(NULL));
	pthread_t tid[10];
	void* res;
	int i;
	for (i = 0; i < 10; ++i){
		pthread_create(&tid[i], NULL, func, NULL);
	}
	for (int i = 0; i < 10; ++i){
		pthread_join(tid[i], &res);
	}
}