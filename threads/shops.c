#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int Shops[5];
int ret1 = 1;
pthread_mutex_t mut[5];

void* buyer1(){
	int ch, nec = rand()%2000+9000;
	int fine;
	while (nec>0){
		fine = 0;
		while (!fine){
			ch = rand()%5;
			if (!pthread_mutex_trylock(&mut[ch])){
				fine = 1;
			}
		}
		nec -= Shops[ch];
		printf ("Buyer1 took %d from %d, and still need %d\n", Shops[ch], ch, nec);
		Shops[ch] = 0;
		pthread_mutex_unlock(&mut[ch]);
		usleep(1000000);
	}
	pthread_exit(&ret1);
	return 0;
}
void* buyer2(){
	int ch, nec = rand()%2000+9000;
	int fine;
	while (nec>0){
		fine = 0;
		while (!fine){
			ch = rand()%5;
			if (!pthread_mutex_trylock(&mut[ch])){
				fine = 1;
			}
		}
		nec -= Shops[ch];
		printf ("Buyer2 took %d from %d, and still need %d\n", Shops[ch], ch, nec);
		Shops[ch] = 0;
		pthread_mutex_unlock(&mut[ch]);
		usleep(1000000);
	}
	pthread_exit(&ret1);
	return 0;
}
void* buyer3(){
	int ch, nec = rand()%2000+9000;
	int fine;
	while (nec>0){
		fine = 0;
		while (!fine){
			ch = rand()%5;
			if (!pthread_mutex_trylock(&mut[ch])){
				fine = 1;
			}
		}
		nec -= Shops[ch];
		printf ("Buyer3 took %d from %d, and still need %d\n", Shops[ch], ch, nec);
		Shops[ch] = 0;
		pthread_mutex_unlock(&mut[ch]);
		usleep(1000000);
	}
	pthread_exit(&ret1);
	return 0;
}

void* adding(){
	int prod = 500, ch;
	int fine;
	while(1){
		fine = 0;
		while (!fine){
			ch = rand()%5;
			if (!pthread_mutex_trylock(&mut[ch])){
				fine = 1;
			}
		}
		Shops[ch] += 500;
		pthread_mutex_unlock(&mut[ch]);
		printf ("We added 500 to %d shop\n\n", ch);
		usleep(2000000);
	}
	pthread_exit(&ret1);
}


int main(){
	pthread_t gruz, client1, client2, client3;
	clock_t start, end;
	double timer;
	void *status;
	int i;
	for (i = 0; i < 5; ++i){
		Shops[i] = rand()%200+900;
	}
	start = clock();
	pthread_create(&gruz, NULL, adding, NULL);
	pthread_create(&client1, NULL, buyer1, NULL);
	pthread_create(&client2, NULL, buyer2, NULL);
	pthread_create(&client3, NULL, buyer3, NULL);
	
	pthread_join(client1, &status);
	pthread_join(client2, &status);
	pthread_join(client3, &status);
	pthread_cancel(gruz);
	end = clock();
	timer = (double)(end-start)/CLOCKS_PER_SEC;
	printf ("Shops program took = %lf\n\n", timer);
	return 0;
}