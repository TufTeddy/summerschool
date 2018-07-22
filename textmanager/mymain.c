#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define clearscr() printf("\033[H\033[J")

int TextEditor(void);
int fileExist(char* path);
int save_file(char* text, char* path);
void malloc_error(void);
char* left(char *string, const int length); 

int main(){
	int mainkey = 0;
	initscr();   	

	keypad(stdscr,TRUE);

	while (1){
		clear();
		cbreak();
		noecho();
		mvaddstr(0, 25, "Welcome to my text editor");
		move(20,0);
        hline(ACS_CKBOARD,80);  
		mvaddstr(21,0,"F1 - open existing file");
		mvaddstr(21,50,"F3 - exit");
		mainkey = getch();
		switch(mainkey){
			case KEY_F(1):
				TextEditor();
				break;
			case KEY_F(3):
				echo();
				nocbreak();
				endwin();
				return 0;
			default:
				break;
		}
	}
}

int TextEditor(){
	clear();
	FILE *fp;
	int maxsize = 600000, maxcols=60, maxrows=10000,startrow=0,startcol=0,
		displayrows=20,returnhandler=1, ky, position=0, row, col, scrollstart=0, x=0, y=0;
	bool allowcr=true;
	char *message=malloc(maxsize*sizeof(char));
	char filepath[500];
	char *path;
	char *where, *original, *savetext,**display;

	bool exitflag=false, ins = true;
	const char *permitted = NULL;
	
	echo();
	nocbreak();
	move (0, 0);
	addstr ("U pressed f1. Now input full path to file: \n");
	getstr(filepath);
	path = filepath;
	noecho();
	cbreak();
	if (!fileExist(path)){
		move (2, 0);
		addstr("No such file\nPress any key to continue..");
		getch();
		return 0;
	} else {
		fp=fopen(path,"r+");
	}
	int i=0;
	char c;
	if(fp!=NULL)
	{
		while((c=fgetc(fp))!=EOF){
			message[i]=(char)c;
			i++;
		}
		fclose(fp);
		move(25, 0);
		hline(ACS_CKBOARD,80);  
		mvaddstr(26,0,"F2 - save  file");
		mvaddstr(26,50,"Esc - exit");
		if ((display = malloc(maxrows * sizeof(char *))) ==NULL)
			malloc_error();	
		for(ky = 0; ky < maxrows; ky++)	if ((display[ky] = malloc((maxcols+1) * sizeof(char)))==NULL)
					malloc_error();

		if ((original=malloc(maxsize*sizeof(char)))==NULL)	
			malloc_error();
		strcpy(original,message);

		if ((savetext=malloc(maxsize*sizeof(char)))==NULL)
			malloc_error();
		strcpy(savetext,message);


		while (!exitflag) {
			int counter; 
			do{
				counter=0;
				where=message;	

				for (row=0; row < maxrows; row++) {
						display[row][0]=127; 
						display[row][1]='\0'; 
				}

				row=0;
				while ((strlen(where) > maxcols || strchr(where,'\n') != NULL) && 
						(display[maxrows-1][0]==127 || display[maxrows-1][0]=='\n')){
							strncpy(display[row],where,maxcols);  
							display[row][maxcols]='\0';          
							if (strchr(display[row],'\n') != NULL)  
									left(display[row],strchr(display[row],'\n')-display[row]);
							else									
								left(display[row],strrchr(display[row],' ')-display[row]);
								if (display[maxrows-1][0]==127 || display[maxrows-1][0]=='\n'){
									where+=strlen(display[row]);
									if (where[0]=='\n' || where[0]==' ' || where[0]=='\0')    
										where++;	 
									row++;	
								}
				}
				if (row == maxrows-1 && strlen(where) > maxcols){
					strcpy(message,savetext);	
					if (ky==127)	
						position++;
					counter=1;			
				}
			} while (counter);
			strcpy(display[row],where);

			ky=0;
			if (strchr(display[maxrows-1],'\n') != NULL)	
				if (strchr(display[maxrows-1],'\n')[1] != '\0')	
							ky=127;
			col=position;		
			row=0;				
			counter=0;			
			while (col > strlen(display[row])){
					col-=strlen(display[row]);	
					counter+=strlen(display[row]);
					if (message[counter] ==' ' || message[counter]=='\n' || message[counter]=='\0'){
						col--;	
						counter++;	
					}
					row++;
			}
			if (col > maxcols-1){
				row++;	
				col=0;
			}

			if (!ky){
				if (row < scrollstart)
						scrollstart--; 
				if (row > scrollstart+displayrows-1) 
					scrollstart++;	
				for (counter=0;counter < displayrows; counter++){
					mvhline(counter+startrow,startcol,' ',maxcols);	
					if (display[counter+scrollstart][0] != 127)		
						mvprintw(counter+startrow,startcol,"%s",display[counter+scrollstart]);
				}
				move(row+startrow-scrollstart,col+startcol);

				ky=getch();
			}

			switch(ky){
				case 27: 
					exitflag=true;
					//exit(1);		
					break;
				case KEY_F(2):  
					getyx(stdscr,y,x);
					save_file(message, path);	
					exitflag = true;
					move(y,x);
					break;
				case KEY_LEFT:	
					if (position)	
						position--;	
					break;
				case KEY_RIGHT:	
					if (position < strlen(message) && (row != maxrows-1 || col < maxcols-1))
						position++;
					break;
				case KEY_UP:
					if (row){
						if (col > strlen(display[row-1]))
							position=strlen(display[row-1]);
						else
							position=col;
						ky=0;	//set ky=0
						for (col=0; col < row-1; col++)	{
							position+=strlen(display[col]);	
							ky+=strlen(display[col]);
							if ((strlen(display[col]) < maxcols) || 
								(message[ky]==' ' && strlen(display[col])==maxcols)){
									position++;	
									ky++;
							}
						}
					}
					break;
				case KEY_DOWN:
					if (row < maxrows-1)
						if (display[row+1][0] !=127){
							if (col < strlen(display[row+1]))	
								position=col;
							else
								position=strlen(display[row+1]);
							ky=0;//set ky=0;
							for (col=0; col <= row; col++){
								position+=strlen(display[col]);
								ky+=strlen(display[col]);
								if ((strlen(display[col]) < maxcols) || (message[ky]==' ' 
									&& strlen(display[col])==maxcols)){
										position++;
										ky++;
								}
							}
						}
							break;
					case KEY_IC: 
							ins=!ins;
							if (ins)
									curs_set(2);
							else
									curs_set(1);
							break;
					case 330:
							if (strlen(message))
							{
									strcpy(savetext,message);
									memmove(&message[position],&message[position+1],maxsize-position);}
							break;
					case KEY_BACKSPACE:			
						if (strlen(message) && position){
							strcpy(savetext,message);
							position--;
							memmove(&message[position],&message[position+1],maxsize-position);
						}
						break;
					default: 
						if (((permitted==NULL && ky > 31 && ky < 127) || 
							(permitted != NULL && strchr(permitted,ky))) && 
							strlen(message) < maxsize-1 && (row !=maxrows-1 || 
							(strlen(display[maxrows-1]) < maxcols || 
							(ins && (row!=maxrows-1 && col < maxcols ))))) {
							if (ins || message[position+1]=='\n' || message[position]== '\n')    
								memmove(&message[position+1],&message[position],maxsize-position);
							message[position]=ky;
							if (row != maxrows-1 || col < maxcols-1)
								position++;
						}
					}
					if(!allowcr)
					if (message[0]=='\n')
					{
							memmove(&message[0],&message[1],maxsize-1);
							if (position)
									position--;
					 }
		}
		
		free(original);
		free(savetext);
		for(scrollstart = 0; scrollstart < maxrows; scrollstart++)
			free(display[scrollstart]);
		free(display);
		endwin();
		return 0; 
	}
}

int fileExist(char* path)
{
     FILE *fp = fopen(path,"r");   
     if(fp==NULL)                   
        return 0;
     fclose(fp);   
     return 1;
}

int save_file(char* text, char* path) { 
	int sp, i=0;
    FILE* fps;	
    move(33, 0);
    
	fps=fopen(path,"w+");
	fprintf(fps,"%s",text);
    move(33, 0);deleteln();flash();
    addstr("\n\nFile saved. Press any key to continue..\n\n");
	getch();
    fclose(fps);
    return 1;
}

void malloc_error(void)      
{
	endwin();
	fprintf(stderr, "malloc error:out of memory\n");
	exit(EXIT_FAILURE);
}

char* left(char *string, const int length)
{
	if (strlen(string) > length)   
		string[length]='\0';
	return string;
}
