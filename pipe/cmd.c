#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int command(int, int, int);
void cleanup(int);
void split(char*);
int run(char*, int, int, int);
char* skipwhite(char*);

char line[1024];
int n = 0;
char* args[512];
pid_t pid;
int command_pipe[2];
 
int main(){
	printf("cmd line, type'exit' to exit\n");
	while (1) {
		printf("$ ");
		fflush(NULL);
 
		if (!fgets(line, 1024, stdin)) 
			return 0;
 
		int input = 0;
		int first = 1;
 
		char* cmd = line;
		char* next = strchr(cmd, '|');
 
		while (next != NULL) {
			*next = '\0';
			input = run(cmd, input, first, 0);
 
			cmd = next + 1;
			next = strchr(cmd, '|');
			first = 0;
		}
		input = run(cmd, input, first, 1);
		cleanup(n);
		n = 0;
	}
	return 0;
}

int run(char* cmd, int input, int first, int last)
{
	split(cmd);
	if (args[0] != NULL) {
		if (strcmp(args[0], "exit") == 0) 
			exit(0);
		n += 1;
		return command(input, first, last);
	}
	return 0;
}

void split(char* cmd)
{
	cmd = skipwhite(cmd);
	char* next = strchr(cmd, ' ');
	int i = 0;
 
	while(next != NULL) {
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = skipwhite(next + 1);
		next = strchr(cmd, ' ');
	}
 
	if (cmd[0] != '\0') {
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		++i; 
	}
 
	args[i] = NULL;
}

char* skipwhite(char* s)
{
	while (isspace(*s))
		++s;
	return s;
}

int command(int input, int first, int last)
{
	int fd[2];
 
	pipe( fd );	
	pid = fork();

	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) {
			dup2( fd[1], STDOUT_FILENO );
		} else if (first == 0 && last == 0 && input != 0) {
			dup2(input, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
		} else {
			dup2( input, STDIN_FILENO );
		}
 
		if (execvp( args[0], args) == -1)
			exit(EXIT_FAILURE);
	}
 
	if (input != 0) 
		close(input);

	close(fd[1]);
 
	if (last == 1)
		close(fd[0]);
 
	return fd[0];
}
 
void cleanup(int n)
{
	int i;
	for (i = 0; i < n; ++i) 
		wait(NULL); 
}
 

