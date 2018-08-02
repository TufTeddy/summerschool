#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int fd[2];
	pid_t pid;
	char inputln[128];
	pipe(fd);
	printf ("cmd line, type 'exit' to exit\n");
	while (1){
		printf("cmd: ");
		scanf("%s", inputln);
		
	}
}
