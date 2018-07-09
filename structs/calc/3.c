#include "func.h"

int main(){
	int choice;
	while (1){
		printf ("\n\n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n0.Exit\n");
		scanf ("%d", &choice);
		switch(choice){
			case 1:
				clear();
				input();
				add();
				break;
			case 2:
				clear();
				input();
				sub();
				break;
			case 3:
				clear();
				input();
				mult();
				break;
			case 4:
				clear();
				input();
				divis();
				break;
			case 0: 
				exit(1);
		}
	}
	
	return 0;
}