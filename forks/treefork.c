#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	pid_t idpid01, idpid02, idpid11, idpid12, idpid21, taker;
	int status;
	idpid01 = fork();
	if (idpid01 == -1){
		printf ("Error with fork\n");
		return 0;
	} else {
		if (idpid01 == 0){
			printf ("1. Level1, pid = %d, ppid = %d\n",
				 getpid(), getppid());
			idpid11 = fork();
			if (idpid11 == 0){
				printf ("1. Level2 From 1.L1 , pid = %d, ppid = %d\n",
						getpid(), getppid());
			} else {
				idpid12 = fork();
				if (idpid12 == 0){
					printf ("2. Level2 From 1.L1 second, pid = %d, ppid = %d\n",
							getpid(), getppid());
				}
				taker = wait(&status);
				taker = wait(&status);
			}
		} else {
			printf ("I'm the batya, pid = %d, ppid = %d\n",
				getpid(), getppid());
			idpid02 = fork();
			if (idpid02 == 0){
				printf ("2. Level1, pid = %d, ppid = %d\n",
						getpid(), getppid());
				idpid21 = fork();
				if (idpid21 == 0){
					printf ("1. Level2 From 2.L1, pid = %d, ppid = %d\n",
							getpid(), getppid());
				}
				taker = wait(&status);
			}
			taker = wait(&status);
			taker = wait(&status);
		}
	}
			

}
