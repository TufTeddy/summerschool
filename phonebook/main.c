#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

struct People{
	char *Name;
	char *SecondName;
	char *Phone;
	struct People *head, *next;
};

void Menu(void);
void Input (struct People *pers);
void Show (struct People *pers);
void Search(struct People *pers);
void Del (struct People *pers);
int main(){
	int key;
	struct People *pers = malloc(sizeof(struct People));
	pers->head = NULL;
	while (1){
		Menu();
		scanf("%d", &key);
		switch(key){
			case 1:
				clear();
				Input(pers);
				clear();
				break;
			case 2:
				clear();
				Show(pers);
				break;
			case 3:
				clear();
				Search(pers);
				break;
			case 4:
				clear();
				Del(pers);
				break;
			default:
				exit(0);
		}
	}
}


void Menu(){
	printf("PhonebookMenu:\n"
		"1. Input new Person\n"
		"2. Show List of Persons\n"
		"3. Search with secondname\n"
		"4. Delete last added element\n"
		"Press other button to exit\n");
}

void Input(struct People *pers){
	char *firstname;
	firstname = (char*)malloc(sizeof(char)*20);
	char *secondname;
	secondname = (char*)malloc(sizeof(char)*20);
	char *phone;
	phone = (char*)malloc(sizeof(char)*20);
	printf ("Input name: ");
	scanf("%20s", firstname);
	printf ("Input SecondName: ");
	scanf("%20s", secondname);
	printf ("Input Phone number: ");
	scanf("%20s", phone);
	struct People *temp = malloc(sizeof(struct People));
	temp -> Name = firstname;
	temp -> SecondName = secondname;
	temp -> Phone = phone;
	temp -> next = pers -> head;
	pers -> head = temp;
}

void Show(struct People *pers){
	struct People *List = pers -> head;
	while (List != NULL){
		printf("Name: %s\n", List -> Name);
		printf("SecondName: %s\n", List -> SecondName);
		printf("Phone Number: %s\n\n", List -> Phone);
		List = List -> next;
	}
	if (pers -> head == NULL){
		clear();
		printf ("Empty book :(\n\n");
	}
}

void Search(struct People *pers){
	struct People *List = pers -> head;
	char *secondname;
	int delkey;
	int n;
	printf ("Input number of comparing letters in secondname: ");
	scanf ("%d", &n);
	
	secondname = (char*)malloc(sizeof(char)*n);
	printf ("Input SecondName: ");
	scanf("%10s", secondname);
	if (pers -> head == NULL){
		clear();
		printf ("Empty book :(\n\n");
	}
	
	while (List != NULL){
		if (memcmp (List -> SecondName, secondname, n) == 0){
			clear();
			printf("Name: %s\n", List -> Name);
			printf("SecondName: %s\n", List -> SecondName);
			printf("Phone Number: %s\n\n", List -> Phone);
			printf("press any key to continue");
			scanf ("%d", &delkey);
		}
		
		List = List -> next;
	}
}

void Del(struct People *pers){
	
	if (pers -> head == NULL) {

		printf("Empty book\n");
		return;
	}

	else{

		struct People *Temp_stack = pers -> head -> next;
		pers -> head = NULL;


		pers -> head = Temp_stack;
		printf ("Element deleted\n");

		return;
	}
}

