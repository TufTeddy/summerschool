#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define clearscr() printf("\033[H\033[J")

int TextEditor(void);
int fileExist(char *path);

int main(){
	int mainkey = 0;
	initscr();   	
	//cbreak();
	noecho();
	keypad(stdscr,TRUE);
	clearscr();
	while (1){
		mvaddstr(0, 25, "Welcome to my text editor");
		move(20,0);      // down menu starts from y=20
        hline(ACS_CKBOARD,80);  // placing a horizontal line to separate menu from text area here
///////////////Now we print menu here
		mvaddstr(21,0,"F1 - open existing file");
		mvaddstr(21,50,"F3 - exit");
		mvaddstr(22,50,"F2 to save file");
		mainkey = getch();
		switch(mainkey){
			case KEY_F(1):
				TextEditor();
				break;
			case KEY_F(3):
				endwin();
				exit(1);
			default:
				break;
		}
	}
	return 0;
}

int TextEditor(){
	FILE *fp;
	int maxsize = 100000, maxcols=50, maxrows=2000,startrow=0,startcol=0,
		displayrows=20,returnhandler=1;
	bool allowcr=true;
	char *message=malloc(maxsize*sizeof(char));
	char filepath[500];
	char *path;
	//echo();
	//nocbreak();
	printw ("U pressed f1. Now input full path to file: ");
	scanf ("%s", filepath);
	path = filepath;
	if (!fileExist(path)){
		printf ("\nNo such file in system, try again\n\n");
		return 0;
	} else {
		fp=fopen(path,"r+");
	}
	int i=0;/*just to count*/
	char c;/*just to read a character from file */
	if(fp!=NULL)/*if fp is pointing to a file*/
	{
		while((c=fgetc(fp))!=EOF){
			message[i]=(char)c;
			i++;
		}
		fclose(fp);
	}
	
	free (message);
	return 0;
}

int fileExist(char* path)
{
     FILE *fp = fopen(path,"r");   // open file in read mode
     if(fp==NULL)                   // couldn't open mean file not existing
        return 0;
     fclose(fp);   //close file if opened means file exists
     return 1;
}