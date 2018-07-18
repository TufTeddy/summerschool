#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *load_plugin(const char *pathname)
{
    char    *errmsg;
    void          *handle; /* We deliberately leak the handle */
    char * (*initfunc)(void);

    if (!pathname || !*pathname)
        return "No path specified";

    dlerror();
    handle = dlopen(pathname, RTLD_NOW | RTLD_LOCAL);
	/*if (!handle){
		printf ("No such file or directory\n");
	}*/
    errmsg = dlerror();
    if (errmsg)
        return errmsg;

    initfunc = dlsym(handle, "register_plugin");
	char *funcname;
	funcname = (char*)malloc(sizeof(char)*6);
	funcname = initfunc();
    errmsg = dlerror();
    if (errmsg)
        return errmsg;
	printf("1. %s\n", funcname);

    return funcname;
}

int main(){
	unsigned int n, i;
	printf ("How much librarys you want to include?: ");
	scanf("%d", &n);
	char **Libs;
    Libs = (char**)malloc(n * sizeof(char*));
    for (i = 0; i < n; ++i){
		char str[10] = "./plugins/";
		Libs[i] = (char*)malloc(sizeof(char)*25);
		printf ("Input %d library with '.so' "  , i+1);
		scanf("%25s", Libs[i]);
		char path[34];
		snprintf (path, sizeof path, "%s%s", str, Libs[i]);
		char *last;
		last = (char*)malloc(sizeof(char)*6);
		last = load_plugin (path);
		printf ("%s\n", last);
    }

}