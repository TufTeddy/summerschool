#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t child, father;
	int status;
	child = fork();
	if (!child){
		printf("Child is here\n");
	} else {
		printf("batya here\n");
		father = wait(&status);
	}
	return 0;
}
