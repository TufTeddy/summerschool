#include <stdio.h>
#include <stdlib.h>

int main(){
    int n, i = 0, j = 0, input=1, dir = 0, cntup = 0, cntdwn = 0,
            cntlft = 0, cntrgt = 0;
    int **A;
    scanf("%d", &n);
    A = (int**)malloc(n * sizeof(int*));
    for (i = 0; i < n; ++i){
        A[i] = (int*)malloc(n * sizeof(int));
        for (j = 0; j < n; ++j){
            A[i][j] = 0;
        }
    }
    printf("Start\n\n");
    i = 0;
    j = 0;
    while (input <= n*n){
        if (dir == 0){
            for (j; j < n-cntrgt; ++j){
                A[i][j] = input;
                input++;
            }
            j--;
            i++;
            cntup++;
            dir = 1;
        }
        if (dir == 1){
            for (i; i < n-cntdwn; i++){
                A[i][j] = input;
                input++;
            }
            i--;
            j--;
            cntrgt++;
            dir = 2;
        }
        if (dir == 2){
            for (j; j >= 0+cntlft; --j){
                A[i][j] = input;
                input++;
            }
            j++;
            i--;
            dir = 3;
            cntdwn++;
        }
        if (dir == 3){
            for (i; i >= 0+cntup; --i){
                A[i][j] = input;
                input++;
            }
            i++;
            j++;
            dir = 0;
            cntlft++;
        }
    }
    for (i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            printf ("%d ", A[i][j]);
        }
        printf("\n");
    }
}