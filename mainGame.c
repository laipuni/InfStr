#include<stdlib.h>
#include<curses.h>
#include<unistd.h>
#include "InfStair.h"
#include "outputView.h"

#define ENTER 10
#define START 0
#define RANKING 1
#define HELP 2
#define QUIT 3

void enterMode(int select){
	switch (select) {
		case START: //invoke startGame() in InfStair.c
			startGame();
			break;
		case HELP:
			printw("%d HELP selected", select);
			break;
		case RANKING: //invoke help() in help.c
			printw("%d RANKING selected ", select);
			break;
		case QUIT: //game quit
			endwin();
			exit(0);
			break;
	}
}
int main(){
	initscr();
	crmode();
	keypad(stdscr, TRUE); //for arrow,enter key
	noecho();
	clear();
	curs_set(0);

	//invoke showMainView() in outPutView.c method print mainView with title and three option(start, help, quit);
	showMainView();
	
	int key;
	int select = 0;
	while (1) {
		key = getch();
		switch (key) {
		case KEY_UP:
			select = (select - 1 + 4) % 4;
			setSelectionCursor(select);
			break;
		case KEY_DOWN:
			select = (select + 1) % 4;
			setSelectionCursor(select);
			break;
		case ENTER:
			enterMode(select);
			break;
		}
	}
}
