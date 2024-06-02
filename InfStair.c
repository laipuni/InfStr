#include<curses.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>
#include "stairs.h"
#include "InfStair.h"
#include "outPutView.h"
#include "gameOver.h"

#define SUCESS 1
#define FAIL 0
#define GAME_OVER_TIME 10
#define INIT_TICKTIME 1000

int setTicker(int n_msecs);
void handleFailKey();
void tickEvent();
void CheckKeyDirection(int key);
void countDown();

int score;
int gameOver;
int currentTime;
int tickTime;

int setTicker(int n_msecs) {
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
void handleFailKey() {
	//this func handles GameOver case
	signal(SIGALRM, SIG_IGN);
	gameOver = 1;// gameOver

	do {
		//show gameover Message and Total Score
		showGameOverView(score);
	} while (getch() != 'b');

	char* userName = inputUserName();
	scoreInput(userName,score);
	free(userName);
	clearQueue();
}
void tickEvent() {
	setTimeOverGage(currentTime);
	currentTime--;
	if (currentTime <= 0) {
		handleFailKey();
	}
}
void CheckKeyDirection(int key) {
	if (isCorrectDirection(key)) {
		//if plyer input correct direction key, isCorrectKey(key) is true in stairs.c
		score++;
		currentTime = GAME_OVER_TIME;
		createNewStair();
		showPlayingView();
		setTimeOverGage(currentTime);
	}
	else {
		//if plyer input wrong direction key, isCorrectKey(key) is false in stairs.c
		handleFailKey();
	}
}
void countDown() {
	int time = 3;
	while (time--) {
		printCountDown(time);
		sleep(1);
	}

}
void initVariable() {
	score = 0;
	gameOver = 0;
	tickTime = INIT_TICKTIME;
	currentTime = GAME_OVER_TIME;
}
void startGame(){
	void tickEvent();
	init();//invoke init() in stairs.c
	initVariable();
	signal(SIGALRM, tickEvent);

	//three count down before game start
	countDown();
	showPlayingView();
	setTimeOverGage(currentTime);

	if (setTicker(1000) == -1) {
		perror("[InfStair] fail setTicker");
	}

	int key;
	while (!gameOver) {
		key = getch();
		if (key == KEY_LEFT || key == KEY_RIGHT) {
			CheckKeyDirection(key);
		}
	}

}


