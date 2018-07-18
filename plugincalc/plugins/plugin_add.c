#include <stdlib.h>
#include <stdio.h>

const char *register_plugin(void) __attribute__((used));
const char *register_plugin(void){
    char Add[6] = "Addic";
	Add[6] = '\0';
    char *funcname;
	funcname = (char*)malloc(sizeof(char)*5);
	funcname = Add;
	printf ("2. %s", funcname);
	return funcname;
}