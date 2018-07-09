#include <stdio.h>
#include <stdlib.h>

struct bytes{
	unsigned int b1:1;
	unsigned int b2:1;
	unsigned int b3:1;
	unsigned int b4:1;
	unsigned int b5:1;
	unsigned int b6:1;
	unsigned int b7:1;
	unsigned int b8:1;
};

int main(){
	unsigned int n;
	printf ("Input n\n");
	scanf("%d", &n);
	struct bytes Bytes;
	*(unsigned int*)&Bytes = n;
	printf("%d %d %d %d %d %d %d %d\n\nLet's change 1st bit:\n", 
			Bytes.b8, Bytes.b7, Bytes.b6, Bytes.b5, Bytes.b4, Bytes.b3, Bytes.b2, Bytes.b1);
	if (Bytes.b1 == 1){
		Bytes.b1 = 0;
	} else {
		Bytes.b1 = 1;
	}
	printf("%d %d %d %d %d %d %d %d\n", 
			Bytes.b8, Bytes.b7, Bytes.b6, Bytes.b5, Bytes.b4, Bytes.b3, Bytes.b2, Bytes.b1);
	return 0;
}