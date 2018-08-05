#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void catch(int sig){
	puts("Got SIGINT");
}

int main() {
        struct sigaction sa;

        sigemptyset(&sa.sa_mask);

        sa.sa_flags = 0;
        sa.sa_handler = catch;
	while (1){
        	sigaction(SIGINT, &sa, NULL);
	}
        return 0;
}
