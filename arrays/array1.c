#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	int n, i, tmp;
	printf("Input n\n");
	scanf("%d", &n);
	int *A;
	A = (int*)malloc(n * sizeof(int));
	srand(time(NULL));
	for (i = 0; i < n; ++i){
		A[i] = rand()%50;
		printf("%d ", A[i]);
	}
	printf("\nInversed array:\n");
	for (i = 0; i < n/2; ++i){
		tmp = A[i];
		A[i] = A[n-1-i];
		A[n-1-i] = tmp;
	}
	for (i = 0; i < n; ++i){
		printf ("%d ", A[i]);
	}
	printf("\n");
	return 0;
}
