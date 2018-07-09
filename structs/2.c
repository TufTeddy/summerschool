#include <stdio.h>
#include <stdlib.h>

struct test1{
	char a;
	int b;
};

struct test2{
	char a;
	int b;
} __attribute__((packed));

int main(){
	struct test1 unp = {'a', 4};
	struct test2 pack = {'b', 6};
	printf ("sizeof unpacked = %ld\n", sizeof(unp));
	printf ("sizeof packed = %ld\n", sizeof(pack));
	printf ("unpacked struct = %c, %d\n", unp.a, unp.b);
	printf ("packed struct = %c, %d\n", pack.a, pack.b);
	return 0;
}