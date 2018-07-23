#include <stdio.h>
#include <pthread.h>


void *func(void *ptr){
    int *ptr1 = ptr;
    printf("I'm the %d thread\n", *ptr1);
}

int main(){
    pthread_t tid[5];
    int i;
	void* status;
    int mas[5];
    for (i = 0; i < 5; ++i){
        mas[i] = i;
        pthread_create(&tid[i], NULL, func, &mas[i]);
    }
	for (i = 0; i < 5; ++i){
		pthread_join(tid[i], &status);
	}
	return 0;
}