#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(){
	pid_t pid;
	int fd[2];
	pipe(fd);
	pid = fork();
	if (!pid){
		char output[100];
		close (fd[1]);
		read(fd[0], output, sizeof(output));
		close (fd[0]);
		printf ("Child got %s\n", output);
	} else {
		close(fd[0]);
		char str[100];
		scanf ("%s", str);
		write(fd[1], str, strlen(str)+1);
		close (fd[0]);
	}

}
